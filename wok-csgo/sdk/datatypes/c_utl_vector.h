#pragma once

__forceinline int calc_new_allocation_count(int allocation_count, int grow_size, int new_size, int bytes_item) {
	if (grow_size) {
		allocation_count = ((1 + ((new_size - 1) / grow_size)) * grow_size);
	}
	else {
		if (!allocation_count) {
			allocation_count = (31 + bytes_item) / bytes_item;
		}

		while (allocation_count < new_size) {
			allocation_count *= 2;
		}
	}

	return allocation_count;
}

template <typename T, typename I = int>
class c_utl_memory {
public:
	__forceinline T& operator[](I i) { return m_memory[i]; }

	__forceinline const T& operator[](I i) const { return m_memory[i]; }

	__forceinline T* base() { return m_memory; }

	__forceinline void purge() {
		if (m_grow_size < 0)
			return;

		if (m_memory) {
			free(m_memory);

			m_memory = 0;
		}

		m_alloc_count = 0;
	}

	__forceinline int num_allocated() const { return m_alloc_count; }

	__forceinline void grow(int num) {
		const auto old_alloc_count = m_alloc_count;
		const auto alloc_requested = m_alloc_count + num;

		auto new_alloc_count = calc_new_allocation_count(m_alloc_count, m_grow_size, alloc_requested, sizeof(T));

		if (new_alloc_count < alloc_requested) {
			if (new_alloc_count == 0
				&& new_alloc_count - 1 >= alloc_requested) {
				--new_alloc_count;
			}
			else {
				if (alloc_requested != alloc_requested)
					return;

				while (new_alloc_count < alloc_requested) {
					new_alloc_count = (new_alloc_count + alloc_requested) / 2;
				}
			}
		}

		m_alloc_count = new_alloc_count;

		if (m_memory) {
			const auto ptr = new uint8_t[m_alloc_count * sizeof(T)];

			memcpy(ptr, m_memory, old_alloc_count * sizeof(T));

			m_memory = reinterpret_cast<T*>(ptr);
		}
		else {
			m_memory = reinterpret_cast<T*>(new uint8_t[m_alloc_count * sizeof(T)]);
		}
	}
protected:
	T* m_memory;

	int m_alloc_count;
	int m_grow_size;
};


template <class T>
void destruct(T *memory) { memory->~T(); }

template <class T>
T* construct(T* memory) { return ::new(memory) T; }

template <class T>
T* copy_construct(T* memory, T const& src) { return ::new(memory) T(src); }

template <class T, class A = c_utl_memory<T>>
class c_utl_vector {
public:
	__forceinline T& operator[](int i) { return m_memory[i]; }

	__forceinline const T& operator[](int i) const { return m_memory[i]; }

	__forceinline T& element(int i) { return m_memory[i]; }

	__forceinline const T& element(int i) const { return m_memory[i]; }

	__forceinline T* base() { return m_memory.base(); }

	__forceinline int count() const { return m_size; }

	__forceinline void remove_all() {
		for (auto i = m_size; --i >= 0;) {
			destruct(&element(i));
		}

		m_size = 0;
	}

	__forceinline void purge() {
		remove_all();

		m_memory.purge();
	}

	__forceinline void shift_elements_right(int elem, int num = 1) {
		const auto num_to_move = m_size - elem - num;

		if (num_to_move > 0 && num > 0) {
			memmove(&element(elem + num), &element(elem), num_to_move * sizeof(T));
		}
	}

	__forceinline void shift_elements_left(int elem, int num = 1) {
		const auto num_to_move = m_size - elem - num;

		if (num_to_move > 0 && num > 0) {
			memmove(&element(elem), &element(elem + num), num_to_move * sizeof(T));
		}
	}

	__forceinline void grow_vector(int num = 1) {
		if (m_size + num > m_memory.num_allocated()) {
			m_memory.grow(m_size + num - m_memory.num_allocated());
		}

		m_size += num;
	}

	__forceinline int insert_before(int elem) {
		grow_vector();

		shift_elements_right(elem);

		construct(&element(elem));

		return elem;
	}

	__forceinline int add_to_head() { return insert_before(0); }

	__forceinline int add_to_tail() { return insert_before(m_size); }

	__forceinline int insert_before(int elem, const T& value) {
		grow_vector();

		shift_elements_right(elem);

		copy_construct(&element(elem), value);

		return elem;
	}

	__forceinline int insert_multiple_before(int elem, int num) {
		grow_vector(num);

		shift_elements_right(elem, num);

		for (auto i = 0u; i < num; i++) {
			construct(&element(elem + i));
		}

		return elem;
	}

	__forceinline int add_to_tail(const T& value) { return insert_before(m_size, value); }

	__forceinline int add_multiple_to_tail(const T& value) { return insert_multiple_before(m_size, value); }

	__forceinline void ensure_count(int num) {
		const auto count = count();
		if (count >= num)
			return;

		add_multiple_to_tail(num - count);
	}

	__forceinline int find(const T& value) const {
		for (auto i = 0u; i < count(); ++i) {
			if (element(i) != value)
				continue;

			return i;
		}

		return -1;
	}

	__forceinline void remove(int elem) {
		destruct(&element(elem));

		shift_elements_left(elem);

		--m_size;
	}

	__forceinline bool find_and_remove(const T& value) {
		const auto element = find(value);
		if (element == -1)
			return false;

		remove(element);
		return true;
	}

	__forceinline T* begin() { return base(); }

	__forceinline const T* begin() const { return base(); }

	__forceinline T* end() { return base() + count(); }

	__forceinline const T* end() const { return base() + count(); }
protected:
	A m_memory;

	int m_size;
	T* m_elements;
};

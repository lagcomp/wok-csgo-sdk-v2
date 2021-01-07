#pragma once

namespace memory {
	struct protect_t {
		protect_t(void* ptr, uint32_t size, DWORD flags) {
			m_size = size;
			m_ptr = ptr;

			VirtualProtect(m_ptr, m_size, flags, &m_flags_backup);
		}

		~protect_t() { VirtualProtect(m_ptr, m_size, m_flags_backup, &m_flags_backup); }

		DWORD m_flags_backup = 0ul;
		uint32_t m_size = 0u;
		void* m_ptr = nullptr;
	};

	struct hook_t {
		hook_t() = default;

		hook_t(uintptr_t ptr) {
			m_vtable = reinterpret_cast<uintptr_t**>(ptr);

			init();
		};

		hook_t(void* ptr) {
			m_vtable = reinterpret_cast<uintptr_t**>(ptr);

			init();
		};

		__forceinline bool init() {
			if (!m_vtable)
				return false;

			const auto protect = protect_t(m_vtable, sizeof(uintptr_t), PAGE_READWRITE);

			m_original = *m_vtable;

			for (m_table_length = 0u; m_original[m_table_length]; m_table_length++) {
				if (IS_INTRESOURCE(m_original[m_table_length]))
					break;
			}

			if (!m_table_length)
				return false;

			m_replace = std::make_unique<uintptr_t[]>(m_table_length + 1);

			std::memset(m_replace.get(), 0, m_table_length * sizeof(uintptr_t) + sizeof(uintptr_t));

			std::memcpy(&m_replace[1], m_original, m_table_length * sizeof(uintptr_t));

			std::memcpy(m_replace.get(), &m_original[-1], sizeof(uintptr_t));

			*m_vtable = &m_replace[1];

			return true;
		}

		template <typename T>
		__forceinline void hook(uint32_t index, T replace_function) {
			if (index < 0u
				|| index > m_table_length)
				return;

			m_replace[index + 1u] = reinterpret_cast<uintptr_t>(replace_function);
		}

		template <typename T>
		__forceinline T get_original(uint32_t index) {
			if (index < 0
				|| index > m_table_length)
				return nullptr;

			return reinterpret_cast<T>(m_original[index]);
		}

		__forceinline void unhook(uint32_t index) {
			if (index < 0u
				|| index > m_table_length)
				return;

			m_replace[index + 1u] = m_original[index];
		}

		__forceinline void unhook() {
			if (!m_original)
				return;

			const auto protect = protect_t(m_vtable, sizeof(uintptr_t), PAGE_READWRITE);

			*m_vtable = m_original;
			m_original = nullptr;
		}

		uint32_t m_table_length = 0u;
		uintptr_t** m_vtable = nullptr;
		uintptr_t* m_original = nullptr;
		std::unique_ptr<uintptr_t[]> m_replace = nullptr;
	};

	struct address_t {
		address_t() = default;
		address_t(uint8_t* ptr) { m_ptr = ptr; };
		address_t(uintptr_t* ptr) { m_ptr = reinterpret_cast<uint8_t*>(ptr); };
		address_t(void* ptr) { m_ptr = reinterpret_cast<uint8_t*>(ptr); };

		__forceinline operator uint8_t*() const { return m_ptr; }

		template <typename T>
		__forceinline T cast() const { return (T)(m_ptr); }

		template <typename T = address_t>
		__forceinline T offset(ptrdiff_t value) const { return (T)(m_ptr + value); };

		__forceinline address_t& self_offset(ptrdiff_t value) {
			m_ptr += value;

			return *this;
		}

		template <typename T = address_t>
		__forceinline T rel8(ptrdiff_t offset = 0x1) const { return (T)(m_ptr + offset + sizeof(uint8_t) + *reinterpret_cast<char*>(m_ptr + offset)); }

		__forceinline address_t& self_rel8(ptrdiff_t offset = 0x1) {
			m_ptr = rel8(offset);

			return *this;
		}

		template <typename T = address_t>
		__forceinline T rel32(ptrdiff_t offset = 0x1) const { return (T)(m_ptr + offset + sizeof(uintptr_t) + *reinterpret_cast<ptrdiff_t*>(m_ptr + offset)); }

		__forceinline address_t& self_rel32(ptrdiff_t offset = 0x1) {
			m_ptr = rel32(offset);

			return *this;
		}

		template <typename T = address_t>
		__forceinline T find_opcode(uint8_t opcode, ptrdiff_t offset = 0x0) const {
			auto ptr = m_ptr;

			while (const auto it = *ptr) {
				if (opcode == it)
					break;

				ptr += 1u;
			}

			ptr += offset;

			return (T)(ptr);
		}

		__forceinline address_t& self_find_opcode(uint8_t opcode, ptrdiff_t offset = 0x0) {
			m_ptr = find_opcode(opcode, offset);

			return *this;
		}

		uint8_t* m_ptr = nullptr;
	};

	struct stack_t : public address_t {
		stack_t(uint8_t* ptr = reinterpret_cast<uint8_t*>(_AddressOfReturnAddress()) - sizeof(uintptr_t)) { m_ptr = ptr; }

		__forceinline address_t ret() const { return *reinterpret_cast<uint8_t**>(m_ptr + sizeof(uintptr_t)); }

		__forceinline address_t addr_of_ret() const { return m_ptr + sizeof(uintptr_t); }

		__forceinline stack_t next() const { return *reinterpret_cast<uint8_t**>(m_ptr); }

		__forceinline stack_t& self_next() {
			m_ptr = next();

			return *this;
		}

		template <typename T = address_t>
		__forceinline T local(ptrdiff_t offset) const { return (T)(m_ptr - offset); }

		template <typename T = address_t>
		__forceinline T arg(ptrdiff_t offset) const { return (T)(m_ptr + offset); }
	};

	using headers_t = std::pair<IMAGE_DOS_HEADER*, IMAGE_NT_HEADERS*>;

	struct module_t {
		module_t() = default;
		module_t(LDR_DATA_TABLE_ENTRY* ldr_entry, const headers_t& headers) {
			m_ldr_entry = ldr_entry;

			m_dos_header = std::get<IMAGE_DOS_HEADER*>(headers);
			m_nt_headers = std::get<IMAGE_NT_HEADERS*>(headers);
		}

		__forceinline address_t get_base() const { return m_ldr_entry->DllBase; }

		__forceinline std::string get_name() const {
			if (!m_ldr_entry)
				return "";

			const auto base_dll_name = reinterpret_cast<UNICODE_STRING*>(&m_ldr_entry->Reserved4[0]);

			const auto name = std::wstring(base_dll_name->Buffer, base_dll_name->Length >> 1);

			auto ret = utils::to_utf8(name);

			std::transform(ret.begin(), ret.end(), ret.begin(), tolower);

			return ret;
		}

		IMAGE_DOS_HEADER* m_dos_header = nullptr;
		IMAGE_NT_HEADERS* m_nt_headers = nullptr;

		LDR_DATA_TABLE_ENTRY* m_ldr_entry = nullptr;
	};

	template <typename T>
	__forceinline T get_vfunc(void* base, int index) { return (*static_cast<T**>(base))[index]; }

	headers_t get_file_headers(address_t addr);

	void get_all_modules();

	std::vector<int> sig_to_bytes(const char* sig);

	address_t find_sig(uint32_t offset, const char* sig, uint32_t range = 0u);

	address_t find_module_sig(uint32_t hash, const char* sig);

	address_t get_export(uint32_t module_hash, uint32_t export_hash);

	extern std::unordered_map<uint32_t, module_t> m_modules;
}

#define SIG(module_name, sig) memory::find_module_sig(FNV1A(module_name), _(sig))
#define EXPORT(module_name, export_name) memory::get_export(FNV1A(module_name), FNV1A(export_name))

#define VFUNC(func, index, type, ...) __forceinline auto func { return memory::get_vfunc<type>(this, index)(this, __VA_ARGS__); };
#define VFUNC_SIG(func, module_name, sig, type, ...) __forceinline auto func { static const auto fn = SIG(module_name, sig).cast<type>(); return fn(this, __VA_ARGS__); };

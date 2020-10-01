#pragma once

template <typename T>
class c_singleton {
protected:
	c_singleton() = default;
	~c_singleton() = default;

	c_singleton(const c_singleton&) = delete;
	c_singleton& operator=(const c_singleton&) = delete;

	c_singleton(c_singleton&&) = delete;
	c_singleton& operator=(c_singleton&&) = delete;
public:
	static T* instance() {
		static auto instance = T();

		return &instance;
	}
};
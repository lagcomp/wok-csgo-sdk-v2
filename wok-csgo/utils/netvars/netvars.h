#pragma once
#include "../../sdk/datatypes/recv_prop.h"

namespace netvars {
	void init();

	void dump_recursive(const char* base_class, recv_table_t* table, uint32_t offset);

	template<typename T>
	__declspec(noinline) static T get(uint32_t hash) { return std::get<T>(m_list.at(hash)); }

	extern std::unordered_map<uint32_t, std::pair<recv_prop_t*, uint32_t>> m_list;
};

#define NETVAR(func, type, name) \
	__forceinline type& func { \
		static const auto offset = netvars::get<uint32_t>(HASH(name)); \
		return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); \
	}

#define ANETVAR(func, type, size, name) \
	__forceinline std::array<type, size>& func { \
		static const auto offset = netvars::get<uint32_t>(HASH(name)); \
		return *reinterpret_cast<std::array<type, size>*>(reinterpret_cast<uintptr_t>(this) + offset); \
	}

#define PNETVAR(func, type, name) \
	__forceinline type* func { \
		static const auto offset = netvars::get<uint32_t>(HASH(name)); \
		return reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); \
	}

#define NETVAR_OFFSET(func, type, name, add) \
	__forceinline type& func { \
		static const auto offset = netvars::get<uint32_t>(HASH(name)); \
		return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset + add); \
	}

#define PNETVAR_OFFSET(func, type, name, add) \
	__forceinline type* func { \
		static const auto offset = netvars::get<uint32_t>(HASH(name)); \
		return reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset + add); \
	}

#define MNETVAR(func, type, name, modifier) \
	__forceinline type& func { \
		static const auto offset = netvars::get<uint32_t>(HASH(name)); \
		return **reinterpret_cast<type**>(reinterpret_cast<uintptr_t>(this) + offset * modifier); \
	}

#define NETPROP(func, name) \
	__forceinline static recv_prop_t* func { \
		static const auto prop = netvars::get<recv_prop_t*>(HASH(name)); \
		return prop; \
	}

#define OFFSET(func, type, offset) __forceinline type& func { return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); }

#define POFFSET(func, type, offset) __forceinline type* func { return reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); }

#define PPOFFSET(func, type, offset) __forceinline type& func { return **reinterpret_cast<type**>(reinterpret_cast<uintptr_t>(this) + offset); }

#define MOFFSET(func, type, offset, modifier) __forceinline type& func { return **reinterpret_cast<type**>(reinterpret_cast<uintptr_t>(this) + offset * modifier); }
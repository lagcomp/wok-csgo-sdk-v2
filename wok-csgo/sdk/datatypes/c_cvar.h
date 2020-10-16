#pragma once

using change_callback_t = void(__cdecl*)(void*, const char*, float);

class c_cvar {
public:
	VFUNC(get_name(), 5, const char*(__thiscall*)(void*))
	VFUNC(get_string(), 11, const char*(__thiscall*)(void*))
	VFUNC(set_value(const char* value), 14, void(__thiscall*)(void*, const char*), value)
	VFUNC(set_value(float value), 15, void(__thiscall*)(void*, float), value)
	VFUNC(set_value(int value), 16, void(__thiscall*)(void*, int), value)

	__forceinline char* get_default() { return m_default_value; }

	__forceinline bool get_bool() { return get_int(); }

	__forceinline float get_float() {
		const auto xored_value = *reinterpret_cast<int*>(&m_float_value);
		auto dexored_value = static_cast<int>(xored_value ^ reinterpret_cast<uintptr_t>(this));
		return *reinterpret_cast<float*>(&dexored_value);
	}

	__forceinline int get_int() {
		const auto xored_value = *reinterpret_cast<int*>(&m_int_value);
		auto dexored_value = static_cast<int>(xored_value ^ reinterpret_cast<uintptr_t>(this));
		return *reinterpret_cast<int*>(&dexored_value);
	}

	char							pad0[4];
	c_cvar*							m_next;
	int								m_registered;
	char*							m_name;
	char*							m_help_string;
	bit_flag_t<uint32_t>			m_flags;
	char							pad1[4];
	c_cvar*							m_parent;
	char*							m_default_value;
	char*							m_string_value;
	int								m_string_length;
	float							m_float_value;
	int								m_int_value;
	int								m_has_min;
	float							m_min_value;
	int								m_has_max;
	float							m_max_value;
	c_utl_vector<change_callback_t>	m_callbacks;
};

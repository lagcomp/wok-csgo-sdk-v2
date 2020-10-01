#pragma once

struct d_variant {
	union {
		float		m_float;
		long		m_int;
		char*		m_string;
		void*		m_data;
		vec3_t		m_vec3;
		int64_t		m_int64;
	};

	int				m_type;
};

class c_recv_prop;

struct recv_proxy_data_t {
	const c_recv_prop*	m_recv_prop;
	d_variant			m_value;
	int					m_element;
	int					m_object_id;
};

using recv_var_proxy_fn = void(__cdecl*)(recv_proxy_data_t*, void*, void*);

enum e_send_prop_type : int {
	DPT_INT = 0,
	DPT_FLOAT,
	DPT_VECTOR,
	DPT_VECTORXY,
	DPT_STRING,
	DPT_ARRAY,
	DPT_DATATABLE,
	DPT_INT64,
	DPT_NUMSENDPROPTYPES
};

class c_recv_table;

class c_recv_prop {
public:
	char*				m_var_name;
	e_send_prop_type	m_recv_type;
	int					m_flags;
	int					m_string_buffer_size;
	bool				m_inside_array;
	const void*			m_extra_data;
	c_recv_prop*		m_array_prop;
	void*				m_array_length_proxy;
	recv_var_proxy_fn	m_proxy_fn;
	void*				m_data_table_proxy_fn;
	c_recv_table*		m_data_table;
	int					m_offset;
	int					m_element_stride;
	int					m_elements;
	const char*			m_parent_array_prop_name;
};

class c_recv_table {
public:
	c_recv_prop*	m_props;
	int				m_props_count;
	void*			m_decoder;
	char*			m_net_table_name;
	bool			m_initialized;
	bool			m_in_main_list;
};

struct recv_prop_hook_t {
	recv_prop_hook_t(c_recv_prop* prop) { m_prop = prop; }

	__forceinline void hook(recv_var_proxy_fn proxy_fn) {
		m_original = m_prop->m_proxy_fn;
		m_prop->m_proxy_fn = proxy_fn;
	}

	__forceinline void unhook() { m_prop->m_proxy_fn = m_original; }

	recv_var_proxy_fn get_original() { return m_original; }

	c_recv_prop* m_prop;
	recv_var_proxy_fn m_original;
};
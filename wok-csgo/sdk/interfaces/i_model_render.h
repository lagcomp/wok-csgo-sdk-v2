#pragma once

struct model_t {
	void*						m_file_handle;
	char						m_name[MAX_OS_PATH];
	bit_flag_t<uint32_t>	m_load_flags;
	int						m_server_count;
	int						m_type;
	bit_flag_t<uint32_t>	m_flags;
	vec3_t					m_mins;
	vec3_t					m_maxs;
	float						m_radius;
	c_key_values*			m_key_values;
	char						pad1[24];
};

struct model_render_info_t {
	vec3_t					m_origin;
	qangle_t					m_angles;
	char						pad[4];
	i_client_renderable*	m_renderable;
	const model_t*			m_model;
	const matrix3x4_t*	m_model_to_world;
	const matrix3x4_t*	m_lighting_offset;
	const vec3_t*			m_lighting_origin;
	bit_flag_t<uint32_t>	m_flags;
	int						m_index;
	int						m_skin;
	int						m_body;
	int						m_hitbox_set;
	uint16_t					m_instance;
};

struct studiohwdata_t;

struct draw_model_state_t {
	studiohdr_t*			m_studio_hdr;
	studiohwdata_t*		m_studio_hw_data;
	i_client_renderable*	m_renderable;
	const matrix3x4_t*	m_bones;
};

class i_model_render {
public:
	VFUNC(forced_material_override(i_material* material = nullptr, e_override_type type = OVERRIDE_NORMAL), 1, void(__thiscall*)(void*, i_material*, e_override_type, int), material, type, 0)
	VFUNC(is_forced_material_override(), 2, bool(__thiscall*)(void*))
	VFUNC(draw_model_execute(void* context, const draw_model_state_t& state, const model_render_info_t& info, matrix3x4_t* bones), 21, void(__thiscall*)(void*, void*, const draw_model_state_t&, const model_render_info_t&, matrix3x4_t*), context, state, info, bones)
};
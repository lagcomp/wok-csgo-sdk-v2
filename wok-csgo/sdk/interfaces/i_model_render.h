#pragma once

struct model_t {
	void*   m_table;
	char    m_name[260];
	int		m_load_flags;
	int		m_server_count;
	int		m_type;
	int		m_flags;
	vec3_t  m_mins;
	vec3_t  m_maxs;
	float   m_radius;
	char    pad0[28];
};

struct model_render_info_t {
	vec3_t					m_origin;
	angle_t				m_angles;
	char					pad[4];
	i_client_renderable*	m_renderable;
	const model_t*			m_model;
	const matrix3x4_t*		m_model_to_world;
	const matrix3x4_t*		m_lighting_offset;
	const vec3_t*			m_lighting_origin;
	int						m_flags;
	int						m_index;
	int						m_skin;
	int						m_body;
	int						m_hitboxset;
	uint16_t				m_instance;
};

class studiohwdata_t;

struct draw_model_state_t {
	studiohdr_t*			m_studio_hdr;
	studiohwdata_t*			m_studio_hw_data;
	i_client_renderable*	m_renderable;
	const matrix3x4_t*		m_bones;
};

enum e_override_type {
	OVERRIDE_NORMAL,
	OVERRIDE_BUILD_SHADOWS,
	OVERRIDE_DEPTH_WRITE,
	OVERRIDE_SSAO_DEPTH_WRITE,
};

class i_model_render {
public:
	VFUNC(forced_material_override(i_material* material = nullptr, e_override_type type = OVERRIDE_NORMAL), 1, void(__thiscall*)(void*, i_material*, e_override_type, int), material, type, 0)
		VFUNC(is_forced_material_override(), 2, bool(__thiscall*)(void*))
		VFUNC(draw_model_execute(void* context, const draw_model_state_t& state, const model_render_info_t& info, matrix3x4_t* bones), 21, void(__thiscall*)(void*, void*, const draw_model_state_t&, const model_render_info_t&, matrix3x4_t*), context, state, info, bones)
};
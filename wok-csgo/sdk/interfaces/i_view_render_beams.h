#pragma once

struct beam_info_t {
	beam_info_t() {
		m_type = TE_BEAMPOINTS;
		m_segments = -1;
		m_model_name = 0;
		m_halo_name = 0;
		m_model_index = -1;
		m_halo_index = -1;
		m_renderable = true;
		m_flags = 0;
	}

	int					m_type;
	i_client_entity*	m_start_entity;
	int					m_start_attachment;
	i_client_entity*	m_end_entity;
	int					m_end_attachment;
	vec3_t				m_start;
	vec3_t				m_end;
	int					m_model_index;
	const char*			m_model_name;
	int					m_halo_index;
	const char*			m_halo_name;
	float				m_halo_scale;
	float				m_life;
	float				m_width;
	float				m_end_width;
	float				m_fade_length;
	float				m_amplitude;
	float				m_brightness;
	float				m_speed;
	int					m_start_frame;
	float				m_framerate;
	col_t				m_color;
	bool				m_renderable;
	int					m_segments;
	bit_flag_t<uint32_t>	m_flags;
	vec3_t				m_center;
	float				m_start_radius;
	float				m_end_radius;
};

class beam_t;

class i_view_render_beams {
public:
	VFUNC(draw_beam(beam_t* beam), 5, void(__thiscall*)(void*, beam_t*), beam)
	VFUNC(create_beam_points(beam_info_t& info), 9, beam_t*(__thiscall*)(void*, beam_info_t&), info)
	VFUNC(create_beam_ring_points(beam_info_t& info), 11, beam_t*(__thiscall*)(void*, beam_info_t&), info)
};
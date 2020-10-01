#pragma once

enum e_client_frame_stage {
	FRAME_UNDEFINED = -1,
	FRAME_START,

	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,

	FRAME_RENDER_START,
	FRAME_RENDER_END
};

class c_view_setup {
public:
	int			m_x;
	int			m_old_x;
	int			m_y;
	int			m_old_y;
	int			m_width;
	int			m_old_width;
	int			m_height;
	int			m_old_height;
	char		pad0[144];
	float		m_fov;
	float		m_view_model_fov;
	vec3_t		m_origin;
	angle_t	m_angles;
	char		pad1[124];
};

class i_client_mode {
public:

};
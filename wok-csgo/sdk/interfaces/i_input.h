#pragma once

class c_user_cmd {
public:
	VFUNC_SIG(get_check_sum(), "client.dll", "53 8B D9 83 C8", uint32_t(__thiscall*)(void*))

	virtual	~c_user_cmd() = default;

	int						m_command_number;
	int						m_tick_count;
	qangle_t				m_view_angles;
	vec3_t					m_aim_direction;
	vec3_t					m_move;
	bit_flag_t<uint32_t>	m_buttons;
	char					m_impulse;
	int						m_weapon_select;
	int						m_weapon_sub_type;
	int						m_random_seed;
	short					m_mouse_dx;
	short					m_mouse_dy;
	bool					m_predicted;
	char					pad0[24];
};

class c_verified_user_cmd {
public:
	c_user_cmd  m_cmd;
	uint32_t	m_crc;
};

class i_input {
public:
	VFUNC(get_user_cmd(int slot, int sequence_number), 8, c_user_cmd*(__thiscall*)(void*, int, int), slot, sequence_number)

	c_user_cmd* get_user_cmd(int sequence_number) { return &m_commands[sequence_number % MULTIPLAYER_BACKUP]; }
	c_verified_user_cmd* get_verified_user_cmd(int sequence_number) { return &m_verified_commands[sequence_number % MULTIPLAYER_BACKUP]; }

	char					pad0[12];
	bool					m_track_ir_available;
	bool					m_mouse_initialized;
	bool					m_mouse_active;
	char					pad1[158];
	bool					m_camera_in_third_person;
	char					pad2[2];
	vec3_t					m_camera_offset;
	char					pad3[56];
	c_user_cmd*				m_commands;
	c_verified_user_cmd*	m_verified_commands;
};

#pragma once

enum e_cmd_buttons {
	IN_ATTACK		= (1 << 0),
	IN_JUMP			= (1 << 1),
	IN_DUCK			= (1 << 2),
	IN_FORWARD		= (1 << 3),
	IN_BACK			= (1 << 4),
	IN_USE			= (1 << 5),
	IN_CANCEL		= (1 << 6),
	IN_LEFT			= (1 << 7),
	IN_RIGHT			= (1 << 8),
	IN_MOVELEFT		= (1 << 9),
	IN_MOVERIGHT	= (1 << 10),
	IN_ATTACK2		= (1 << 11),
	IN_RUN			= (1 << 12),
	IN_RELOAD		= (1 << 13),
	IN_ALT1			= (1 << 14),
	IN_ALT2			= (1 << 15),
	IN_SCORE			= (1 << 16),
	IN_SPEED			= (1 << 17),
	IN_WALK			= (1 << 18),
	IN_ZOOM			= (1 << 19),
	IN_WEAPON1		= (1 << 20),
	IN_WEAPON2		= (1 << 21),
	IN_BULLRUSH		= (1 << 22),
	IN_GRENADE1		= (1 << 23),
	IN_GRENADE2		= (1 << 24),
	IN_LOOK_SPIN	= (1 << 25)
};

class c_user_cmd {
public:
	virtual		~c_user_cmd() = default;

	int			m_command_number;
	int			m_tick_count;
	qangle_t	m_view_angles;
	vec3_t		m_aim_direction;
	vec3_t		m_move;
	int			m_buttons;
	char		m_impulse;
	int			m_weapon_select;
	int			m_weapon_sub_type;
	int			m_random_seed;
	short		m_mouse_dx;
	short		m_mouse_dy;
	bool		m_predicted;
	char		pad0[24];
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

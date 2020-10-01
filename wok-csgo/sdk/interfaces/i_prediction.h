#pragma once

struct move_data_t {
	bool            m_first_run_of_functions : 1;
	bool            m_game_code_moved_player : 1;
	bool            m_no_air_control : 1;
	c_base_handle	m_player_handle;
	int				m_impulse_command;
	angle_t		m_view_angles;
	angle_t		m_abs_view_angles;
	int				m_buttons;
	int				m_old_buttons;
	vec3_t			m_move;
	float			m_max_speed;
	float			m_client_max_speed;
	vec3_t			m_velocity;
	vec3_t			m_old_velocity;
	char			pad0[4];
	angle_t        m_angles;
	angle_t		m_old_angles;
	float			m_out_step_height;
	vec3_t			m_out_wish_velocity;
	vec3_t			m_out_jump_velocity;
	vec3_t			m_constraint_center;
	float			m_constraint_radius;
	float			m_constraint_width;
	float			m_constraint_speed_factor;
	bool            m_constraint_past_radius;
	vec3_t			m_abs_origin;
};

class c_base_player;

class i_prediction {
public:
	virtual			~i_prediction() {};

	virtual void	init										() = 0;
	virtual void	shutdown									() = 0;

	virtual void	update									(int start_frame, bool valid_frame, int incoming_acknowledged, int outgoing_command) = 0;

	virtual void	on_received_uncompressed_packet	() = 0;

	virtual void	pre_entity_packet_received			(int commands_acknowledged, int current_world_update_packet, int server_ticks_elapsed) = 0;
	virtual void	post_entity_packet_received		() = 0;
	virtual void	post_network_data_received			(int commands_acknowledged) = 0;

	virtual bool	in_prediction							() const = 0;
	virtual bool	is_first_time_predicted				() const = 0;
	virtual int		get_last_acknowledged_cmd_number	() const = 0;

	virtual int		get_incoming_packet_number			() const = 0;

	virtual void	get_view_origin						(vec3_t& origin) = 0;
	virtual void	set_view_origin						(vec3_t& origin) = 0;
	virtual void	get_view_angles						(angle_t& angle) = 0;
	virtual void	set_view_angles						(angle_t& angle) = 0;

	virtual void	get_local_view_angles				(angle_t& angle) = 0;
	virtual void	set_local_view_angles				(angle_t& angle) = 0;

	virtual void	check_moving_on_ground				(c_base_player *player, double frame_time) = 0;
	virtual void	run_command								(c_base_player *player, c_user_cmd *cmd, i_move_helper *helper) = 0;

	virtual void	setup_move								(c_base_player* player, c_user_cmd* cmd, i_move_helper* helper, move_data_t* move) = 0;
	virtual void	finish_move								(c_base_player* player, c_user_cmd* cmd, move_data_t* move) = 0;
	virtual void	set_ideal_pitch						(int slot, c_base_player* player, const vec3_t& origin, const angle_t& angle, const vec3_t& view_height) = 0;

	virtual void	check_error								(int slot, c_base_player* player, int commands_acknowledged) = 0;


	char						pad0[8];
	bool						m_in_prediction;
	char						pad1[1];
	bool						m_engine_paused;
	bool						m_old_cl_predict_value;
	int							m_prev_start_frame;
	int							m_incoming_packet_number;
	float						m_last_server_world_time_stamp;
	bool						m_first_time_predicted;
	char						pad2[3];
	int							m_commands_predicted;
	int							m_server_commands_acknowledged;
	bool						m_prev_ack_had_errors;
	char						pad3[4];
	int							m_incoming_acknowledged;
	bool						m_unknown0;
	char						pad4[3];
	c_utl_vector<c_base_handle> m_prediction_handles;
	bool						m_unknown1;
	char						pad5[3];
	i_global_vars				m_saved_globals;
};

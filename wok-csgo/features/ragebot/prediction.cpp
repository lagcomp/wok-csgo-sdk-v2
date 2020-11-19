#include "../features.h"

void c_prediction::update() {
	if (interfaces::client_state->m_delta_tick <= 0)
		return;

	interfaces::prediction->update(
		interfaces::client_state->m_delta_tick, true,
		interfaces::client_state->m_last_command_ack, interfaces::client_state->m_last_outgoing_command + interfaces::client_state->m_choked_commands
	);
}

void c_prediction::predict(c_cs_player* player, c_user_cmd* cmd) {
	if (!player->is_alive())
		return;

	m_player = player;
	*m_random_seed = cmd->m_random_seed;

	interfaces::prediction->m_in_prediction = true;
	interfaces::prediction->m_first_time_predicted = false;

	interfaces::global_vars->m_cur_time = TICKS_TO_TIME(player->get_tick_base());
	interfaces::global_vars->m_frame_time = player->get_flags().has(FL_FROZEN) ? 0.f : interfaces::global_vars->m_interval_per_tick;

	interfaces::move_helper->set_host(player);

	interfaces::game_movement->start_track_prediction_errors(player);

	interfaces::game_movement->process_movement(player, m_move_data);

	interfaces::prediction->finish_move(player, cmd, m_move_data);

	interfaces::game_movement->finish_track_prediction_errors(player);

	interfaces::move_helper->set_host(nullptr);

	m_player = nullptr;
	*m_random_seed = -1;

	const auto weapon = player->get_active_weapon();
	if (!weapon) {
		m_spread = m_inaccuracy = 0.f;
		return;
	}

	weapon->update_accuracy();

	m_spread = weapon->get_spread();

	m_inaccuracy = weapon->get_inaccuracy();
}

void c_prediction::process(c_cs_player* player, c_user_cmd* cmd) {
	m_backup.store();

	m_player = player;
	*m_random_seed = cmd->m_random_seed;

	interfaces::global_vars->m_cur_time = TICKS_TO_TIME(player->get_tick_base());
	interfaces::global_vars->m_frame_time = interfaces::global_vars->m_interval_per_tick;

	interfaces::game_movement->start_track_prediction_errors(player);

	interfaces::prediction->setup_move(player, cmd, interfaces::move_helper, m_move_data);

	predict(player, cmd);
}

void c_prediction::restore() {
	m_player = nullptr;
	*m_random_seed = -1;

	m_backup.restore();
}

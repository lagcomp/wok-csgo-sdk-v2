#include "../hooks.h"

bool __stdcall hooks::client_mode::create_move::fn(float input_sample_time, c_user_cmd* cmd) {
	static const auto original = m_client_mode->get_original<T>(index);

	if (!cmd
		|| !cmd->m_command_number)
		return original(input_sample_time, cmd);

	g::cmd = cmd;
	g::send_packet = true;
	g::angles::view = cmd->m_view_angles;

	engine_prediction->pre_start();

	engine_prediction->start(g::local, cmd);
	
	{

	}

	engine_prediction->end(g::local, cmd);

	cmd->m_view_angles.sanitize();

	g::angles::real = cmd->m_view_angles;

	movement->fix(g::angles::view, cmd->m_view_angles);

	cmd->m_move.x = std::clamp(cmd->m_move.x, -450.f, 450.f);
	cmd->m_move.y = std::clamp(cmd->m_move.y, -450.f, 450.f);
	cmd->m_move.z = std::clamp(cmd->m_move.z, -320.f, 320.f);

	*reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(_AddressOfReturnAddress()) + 0x14) = g::send_packet;

	return false;
}

void __stdcall hooks::client_mode::override_view::fn(c_view_setup* setup) {
	static const auto original = m_client_mode->get_original<T>(index);

	original(setup);
}
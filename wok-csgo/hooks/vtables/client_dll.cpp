#include "../hooks.h"

void __stdcall hooks::client_dll::frame_stage_notify::fn(e_client_frame_stage stage) {
	static const auto original = m_client_dll->get_original<T>(index);

	original(stage);
}

__declspec (naked) void __stdcall hooks::client_dll::create_move::gate(int sequence_number, float input_sample_frame_time, bool active) {
	__asm {
		push ebp
		mov ebp, esp
		push ebx
		push esp
		push dword ptr [active]
		push dword ptr [input_sample_frame_time]
		push dword ptr [sequence_number]
		call fn
		pop ebx
		pop ebp
		retn 0Ch
	}
}

void __stdcall hooks::client_dll::create_move::fn(int sequence_number, float input_sample_frame_time, bool active, bool& send_packet) {
	static const auto original = m_client_dll->get_original<T>(index);

	original(interfaces::client_dll, sequence_number, input_sample_frame_time, active);

	g::send_packet = send_packet = true;

	if (!g::local)
		return;

	const auto cmd = interfaces::input->get_user_cmd(sequence_number);
	if (!cmd
		|| !cmd->m_command_number)
		return;

	g::cmd = cmd;
	g::angles::view = cmd->m_view_angles;

	movement->set_view_angles(cmd->m_view_angles);

	engine_prediction->update();

	movement->on_create_move(false);

	engine_prediction->process(g::local, cmd);

	{

	}

	engine_prediction->restore();

	cmd->m_view_angles.sanitize();

	g::angles::real = cmd->m_view_angles;

	movement->on_create_move(true);

	send_packet = g::send_packet;

	const auto verified = interfaces::input->get_verified_user_cmd(sequence_number);

	verified->m_cmd = *cmd;
	verified->m_crc = cmd->get_check_sum();
}

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
		lea ecx, [esp]
		push ecx
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

	engine_prediction->update();

	engine_prediction->process(g::local, cmd);

	{

	}

	engine_prediction->restore(g::local, cmd);

	cmd->m_view_angles.sanitize();

	g::angles::real = cmd->m_view_angles;

	movement->fix(g::angles::view, cmd->m_view_angles);
        movement->bhop();
	cmd->m_move.x = math::clamp(cmd->m_move.x, -450.f, 450.f);
	cmd->m_move.y = math::clamp(cmd->m_move.y, -450.f, 450.f);
	cmd->m_move.z = math::clamp(cmd->m_move.z, -320.f, 320.f);

	send_packet = g::send_packet;

	const auto verified = interfaces::input->get_verified_user_cmd(sequence_number);

	verified->m_cmd = *cmd;
	verified->m_crc = cmd->get_check_sum();
}

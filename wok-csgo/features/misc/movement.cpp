#include "../features.h"

void c_movement::fix(const qangle_t& wish, const qangle_t& original) {
	if (wish == original)
		return;

	vec3_t wish_forward, wish_right, wish_up;

	math::angle_vectors(wish, &wish_forward, &wish_right, &wish_up);

	wish_forward.z = wish_right.z = wish_up.x = wish_up.y = 0.f;

	wish_forward.normalize();
	wish_right.normalize();
	wish_up.normalize();

	vec3_t original_forward, original_right, original_up;

	math::angle_vectors(original, &original_forward, &original_right, &original_up);

	original_forward.z = original_right.z = original_up.x = original_up.y = 0.f;

	original_forward.normalize();
	original_right.normalize();
	original_up.normalize();

	const auto unk0 = wish_forward.x * g::cmd->m_move.x;
	const auto unk1 = wish_forward.y * g::cmd->m_move.x;
	const auto unk2 = wish_right.x * g::cmd->m_move.y;
	const auto unk3 = wish_right.y * g::cmd->m_move.y;
	const auto unk4 = wish_up.z * g::cmd->m_move.z;

	g::cmd->m_move.x = original_forward.x * unk2 + original_forward.y * unk3 + original_forward.x * unk0 + original_forward.y * unk1 + original_forward.z * unk4;
	g::cmd->m_move.y = original_right.x * unk2 + original_right.y * unk3 + original_right.x * unk0 + original_right.y * unk1 + original_right.z * unk4;
	g::cmd->m_move.z = original_up.x * unk3 + original_up.y * unk2 + original_up.x * unk1 + original_up.y * unk0 + original_up.z * unk4;
}

void c_movement::bhop()
{
	if (!g::local
	|| !interfaces::engine->is_in_game()
	|| !interfaces::engine->is_connected()
	|| !g::local->is_alive()
	|| g::local->get_water_level() >= 2
	|| IN_RANGE(g::local->get_move_type(), MOVETYPE_NOCLIP, MOVETYPE_OBSERVER)
	|| g::local->get_ground_entity().is_valid()
	|| !(g::cmd->m_buttons & IN_JUMP))
	return;

	g::cmd->m_buttons &= ~IN_JUMP;
}

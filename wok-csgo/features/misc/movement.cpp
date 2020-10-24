#include "../features.h"

void c_movement::fix(const qangle_t& wish, const qangle_t& original) {
	if (wish == original)
		return;

	vec3_t wish_x, wish_y, wish_z;

	math::angle_vectors(wish, &wish_x, &wish_y, &wish_z);

	wish_x.z = wish_y.z = wish_z.x = wish_z.y = 0.f;

	wish_x.normalize();
	wish_y.normalize();
	wish_z.normalize();

	vec3_t original_x, original_y, original_z;

	math::angle_vectors(original, &original_x, &original_y, &original_z);

	original_x.z = original_y.z = original_z.x = original_z.y = 0.f;

	original_x.normalize();
	original_y.normalize();
	original_z.normalize();

	const auto calc_xx = wish_x.x * g::cmd->m_move.x;
	const auto calc_xy = wish_x.y * g::cmd->m_move.x;
	const auto calc_yx = wish_y.x * g::cmd->m_move.y;
	const auto calc_yy = wish_y.y * g::cmd->m_move.y;
	const auto calc_zz = wish_z.z * g::cmd->m_move.z;

	g::cmd->m_move.x = original_x.x * calc_yx + original_x.y * calc_yy + original_x.x * calc_xx + original_x.y * calc_xy + original_x.z * calc_zz;
	g::cmd->m_move.y = original_y.x * calc_yx + original_y.y * calc_yy + original_y.x * calc_xx + original_y.y * calc_xy + original_y.z * calc_zz;
	g::cmd->m_move.z = original_z.x * calc_yy + original_z.y * calc_yx + original_z.x * calc_xy + original_z.y * calc_xx + original_z.z * calc_zz;
}
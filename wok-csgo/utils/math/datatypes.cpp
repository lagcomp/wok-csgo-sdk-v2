#include "../utils.h"

vec3_t vec3_t::transform(const matrix3x4_t& in) const {
	return vec3_t(dot_product(in[0]) + in[0][3], dot_product(in[1]) + in[1][3], dot_product(in[2]) + in[2][3]);
}

vec3_t vec3_t::i_transform(const matrix3x4_t& in) const {
	return vec3_t(
		(x - in[0][3]) * in[0][0] + (y - in[1][3]) * in[1][0] + (z - in[2][3]) * in[2][0],
		(x - in[0][3]) * in[0][1] + (y - in[1][3]) * in[1][1] + (z - in[2][3]) * in[2][1],
		(x - in[0][3]) * in[0][2] + (y - in[1][3]) * in[1][2] + (z - in[2][3]) * in[2][2]
	);
}

vec3_t vec3_t::rotate(const matrix3x4_t& in) const {
	return vec3_t(dot_product(in[0]), dot_product(in[1]), dot_product(in[2]));
}

vec3_t vec3_t::i_rotate(const matrix3x4_t& in) const {
	return vec3_t(
		x * in[0][0] + y * in[1][0] + z * in[2][0],
		x * in[0][1] + y * in[1][1] + z * in[2][1],
		x * in[0][2] + y * in[1][2] + z * in[2][2]
	);
}

void qangle_t::normalize() {
	x = math::clamp(remainderf(x, 360.f), -89.f, 89.f);
	y = math::clamp(remainderf(y, 360.f), -180.f, 180.f);
	z = 0.f;
}

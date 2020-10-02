#include "../utils.h"

namespace math {
	void sin_cos(float rad, float& sin, float& cos) {
		sin = math::sin(rad);
		cos = math::cos(rad);
	}

	void angle_vectors(const qangle_t& angles, vec3_t* forward, vec3_t* right, vec3_t* up) {
		vec3_t cos, sin;

		sin_cos(DEG2RAD(angles.x), sin.x, cos.x);
		sin_cos(DEG2RAD(angles.y), sin.y, cos.y);
		sin_cos(DEG2RAD(angles.z), sin.z, cos.z);

		if (forward) {
			forward->x = cos.x * cos.y;
			forward->y = cos.x * sin.y;
			forward->z = -sin.x;
		}

		if (right) {
			right->x = -sin.z * sin.x * cos.y + -cos.z * -sin.y;
			right->y = -sin.z * sin.x * sin.y + -cos.z * cos.y;
			right->z = -sin.z * cos.x;
		}

		if (up) {
			up->x = cos.z * sin.x * cos.y + -sin.z * -sin.y;
			up->y = cos.z * sin.x * sin.y + -sin.z * cos.y;
			up->z = cos.z * cos.x;
		}
	}

	qangle_t calc_angle(const vec3_t& src, const vec3_t& dst) {
		const auto delta = src - dst;
		if (delta.empty())
			return qangle_t();

		const auto length = delta.length();

		if (delta.z == 0.f && length == 0.f
			|| delta.y == 0.f && delta.x == 0.f)
			return qangle_t();

		auto angles = qangle_t(asin(delta.z / length) * M_RADPI, atan(delta.y / delta.x) * M_RADPI, 0.f);

		if (delta.x >= 0.f) {
			angles.y += 180.f;
		}

		return angles.normalize();
	}
}

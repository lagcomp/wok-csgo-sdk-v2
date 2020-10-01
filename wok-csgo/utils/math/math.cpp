#include "../utils.h"

namespace math {
	void sin_cos(float rad, float& sin, float& cos) {
		sin = math::sin(rad);
		cos = math::cos(rad);
	}

	void angle_vectors(const angle_t& angles, vec3_t* forward, vec3_t* right, vec3_t* up) {
		vec3_t cos, sin;

		sin_cos(deg_to_rad(angles.x), sin.x, cos.x);
		sin_cos(deg_to_rad(angles.y), sin.y, cos.y);
		sin_cos(deg_to_rad(angles.z), sin.z, cos.z);

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

	angle_t calc_angle(const vec3_t& src, const vec3_t& dst) {
		const auto delta = src - dst;
		if (!delta)
			return angle_t();

		const auto length = delta.length();

		if (delta.z == 0.f && length == 0.f
			|| delta.y == 0.f && delta.x == 0.f)
			return angle_t();

		auto angles = angle_t(asin(delta.z / length) * RADPI, atan(delta.y / delta.x) * RADPI, 0.f);

		if (delta.x >= 0.f) {
			angles.y += 180.f;
		}

		return angles.normalize();
	}

	float asin(float x) {
		const auto negate = x < 0.f;

		x = abs(x);

		auto ret = -0.0187293;

		ret *= x;
		ret += 0.0742610;
		ret *= x;
		ret -= 0.2121144;
		ret *= x;
		ret += 1.5707288;
		ret = 3.14159265358979 * 0.5 - sqrt(1.0 - x) * ret;

		return ret - 2.f * negate * ret;
	}

	float atan2(float y, float x) {
		const auto x_abs = fabs(x);
		const auto y_abs = fabs(y);

		auto t3 = static_cast<double>(x_abs);
		auto t1 = static_cast<double>(y_abs);

		auto t0 = max(t3, t1);

		t1 = min(t3, t1);
		t3 = 1 / t0;
		t3 = t1 * t3;

		const auto t4 = t3 * t3;

		t0 = -0.013480470;
		t0 = t0 * t4 + 0.057477314;
		t0 = t0 * t4 - 0.121239071;
		t0 = t0 * t4 + 0.195635925;
		t0 = t0 * t4 - 0.332994597;
		t0 = t0 * t4 + 0.999995630;
		t3 = t0 * t3;

		t3 = y_abs > x_abs ? 1.570796327 - t3 : t3;
		t3 = x < 0.f ? 3.141592654 - t3 : t3;
		t3 = y < 0.f ? -t3 : t3;

		return t3;
	}

	float atan(float x) {
		return atan2(x, 1.f);
	}

	float sin(float x) {
		x *= 0.159155f;
		x -= floor(x);

		const auto xx = x * x;

		auto y = -6.87897;

		y = y * xx + 33.7755;
		y = y * xx - 72.5257;
		y = y * xx + 80.5874;
		y = y * xx - 41.2408;
		y = y * xx + 6.28077;

		return x * y;
	}

	float cos(float x) {
		return sin(x + 1.5708f);
	}

	float acos(float x) {
		const auto negate = x < 0.f;

		x = abs(x);

		auto ret = -0.0187293f;

		ret *= x;
		ret += 0.0742610;
		ret *= x;
		ret -= 0.2121144;
		ret *= x;
		ret += 1.5707288;
		ret *= sqrt(1.0 - x);
		ret -= 2 * negate * ret;

		return negate * 3.14159265358979 + ret;
	}
}
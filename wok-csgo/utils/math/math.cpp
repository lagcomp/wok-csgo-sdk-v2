#include "../utils.h"

namespace math {
	void sin_cos(float radian, float& sin, float& cos) {
		sin = std::sin(radian);
		cos = std::cos(radian);
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

		auto angles = qangle_t(fast_asin(delta.z / length) * M_RADPI, fast_atan(delta.y / delta.x) * M_RADPI, 0.f);

		if (delta.x >= 0.f) {
			angles.y += 180.f;
		}

		return angles.normalized();
	}

	void vector_angles(const vec3_t& forward, qangle_t& angles) {
		auto yaw = 0.f, pitch = 0.f;

		if (forward.z == 0.f
			&& forward.x == 0.f) {
			yaw = 0.f;
			pitch = forward.z > 0.f ? 90.f : 270.f;
		}
		else {
			yaw = fast_atan2(forward.y, forward.x) * 180.f / M_PI;

			if (yaw < 0.f) {
				yaw += 360.f;
			}

			pitch = fast_atan2(-forward.z, forward.length_2d()) * 180.f / M_PI;

			if (pitch < 0.f) {
				pitch += 360.f;
			}
		}

		angles.x = clamp(remainderf(pitch, 360.f), -89.f, 89.f);
		angles.y = clamp(remainderf(yaw, 360.f), -180.f, 180.f);
		angles.z = 0.f;
	}

	float clamp(float value, float min, float max) {
		_mm_store_ss(&value, _mm_min_ss(_mm_max_ss(_mm_set_ss(value), _mm_set_ss(min)), _mm_set_ss(max)));
		return value;
	}

	float fast_asin(float x) {
		const auto negate = float(x < 0);
		x = abs(x);
		auto ret = -0.0187293;
		ret *= x;
		ret += 0.0742610;
		ret *= x;
		ret -= 0.2121144;
		ret *= x;
		ret += 1.5707288;
		ret = 3.14159265358979 * 0.5 - sqrt(1.0 - x)*ret;
		return float(ret - 2 * negate * ret);
	}

	float fast_atan2(const float y, const float x) {
		const auto xabs = fabs(x);
		const auto yabs = fabs(y);
		double t3 = xabs;
		double t1 = yabs;
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

		t3 = (yabs > xabs) ? 1.570796327 - t3 : t3;
		t3 = (x < 0) ? 3.141592654 - t3 : t3;
		t3 = (y < 0) ? -t3 : t3;

		return float(t3);
	}

	float fast_atan(const float x) {
		return fast_atan2(x, float(1));
	}

	float fast_sin(float x) {
		x *= float(0.159155);
		x -= floor(x);
		const auto xx = x * x;
		auto y = -6.87897;
		y = y * xx + 33.7755;
		y = y * xx - 72.5257;
		y = y * xx + 80.5874;
		y = y * xx - 41.2408;
		y = y * xx + 6.28077;
		return float(x * y);
	}

	float fast_cos(const float x) {
		return fast_sin(x + 1.5708f);
	}

	float fast_acos(float x) {
		const auto negate = float(x < 0);
		x = abs(x);
		auto ret = float(-0.0187293);
		ret = ret * x;
		ret = ret + 0.0742610;
		ret = ret * x;
		ret = ret - 0.2121144;
		ret = ret * x;
		ret = ret + 1.5707288;
		ret = ret * sqrt(1.0 - x);
		ret = ret - 2 * negate * ret;
		return negate * 3.14159265358979 + ret;
	}

	float fast_rsqrt(float x) {
		float* ret;
		const auto xx = _mm_load_ss(&a);
		auto xr = _mm_rsqrt_ss(xx);
		auto xt = _mm_mul_ss(xr, xr);
		xt = _mm_mul_ss(xt, xx);
		xt = _mm_sub_ss(_mm_set_ss(3.f), xt);
		xt = _mm_mul_ss(xt, _mm_set_ss(0.5f));
		xr = _mm_mul_ss(xr, xt);
		_mm_store_ss(ret, xr);
		return *ret;
	}
}

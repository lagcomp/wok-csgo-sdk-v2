#pragma once

namespace math {
	constexpr auto PI = 3.14159265358979323846f;
	constexpr auto RADPI = 57.295779513082f;

	__forceinline auto rad_to_deg(const float rad) { return rad * (180.f / PI); }

	__forceinline auto deg_to_rad(const float deg) { return deg * (PI / 180.f); }

	void sin_cos(float rad, float& sin, float& cos);

	__forceinline float clamp(float value, float min, float max) {
		_mm_store_ss(&value, _mm_min_ss(_mm_max_ss(_mm_set_ss(value), _mm_set_ss(min)), _mm_set_ss(max)));
		return value;
	}

	void angle_vectors(const angle_t& angles, vec3_t* forward, vec3_t* right = nullptr, vec3_t* up = nullptr);

	angle_t calc_angle(const vec3_t& src, const vec3_t& dst);

	float asin(float x);

	float atan2(float y, float x);

	float atan(float x);

	float sin(float x);

	float cos(float x);

	float acos(float x);
}
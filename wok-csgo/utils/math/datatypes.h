#pragma once

struct vec2_t {
	vec2_t() = default;
	vec2_t(float ix, float iy) {
		x = ix;
		y = iy;
	}

	float x = 0.f, y = 0.f;

	__forceinline vec2_t operator+(const vec2_t& value) const { return vec2_t(x + value.x, y + value.y); }

	__forceinline vec2_t operator-(const vec2_t& value) const { return vec2_t(x - value.x, y - value.y); }

	__forceinline vec2_t operator+(float value) const { return vec2_t(x + value, y + value); }

	__forceinline vec2_t operator-(float value) const { return vec2_t(x - value, y - value); }

	__forceinline vec2_t operator/(float value) const { return vec2_t(x / value, y / value); }

	__forceinline vec2_t operator*(float value) const { return vec2_t(x * value, y * value); }

	__forceinline vec2_t operator-() const { return vec2_t(-x, -y); }

	__forceinline vec2_t& operator-=(const vec2_t& value) {
		x -= value.x;
		y -= value.y;

		return *this;
	}

	__forceinline vec2_t& operator+=(const vec2_t& value) {
		x += value.x;
		y += value.y;

		return *this;
	}

	__forceinline vec2_t& operator/=(float value) {
		x /= value;
		y /= value;

		return *this;
	}

	__forceinline vec2_t& operator*=(float value) {
		x *= value;
		y *= value;

		return *this;
	}

	__forceinline vec2_t& normalize() {
		auto ret = vec2_t();
		const auto len = length();
		
		if (len != 0.f)
			ret /= len;
		else
			ret.x = ret.y = 0.f;

		return ret;
	}

	__forceinline vec2_t normalized() const {
		auto ret = *this;

		return ret.normalize();
	}

	__forceinline bool operator==(const vec2_t& value) const { return x == value.x && y == value.y; }

	__forceinline bool operator!=(const vec2_t& value) const { return !(operator==(value)); }

	__forceinline float length_sqr() const { return x * x + y * y; }

	__forceinline float length() const { return math::sqrt(length_sqr()); }

	__forceinline float dot_product(const vec2_t& value) const { return x * value.x + y * value.y; }

	__forceinline float dot_product(const float* value) const { return x * value[0] + y * value[1]; }

	__forceinline float dist_to(const vec2_t& value) const { return (*this - value).length(); }

	__forceinline bool is_valid() const { return std::isfinite(x) && std::isfinite(y); }

	__forceinline bool empty() const { return x == 0.f && y == 0.f; }
};

struct qangle_t;
struct matrix3x4_t;

struct vec3_t {
	vec3_t() = default;
	vec3_t(float ix, float iy, float iz) {
		x = ix;
		y = iy;
		z = iz;
	}

	float x = 0.f, y = 0.f, z = 0.f;

	__forceinline vec3_t operator+(const vec3_t& value) const { return vec3_t(x + value.x, y + value.y, z + value.z); }

	__forceinline vec3_t operator-(const vec3_t& value) const { return vec3_t(x - value.x, y - value.y, z - value.z); }

	__forceinline vec3_t operator-(float value) const { return vec3_t(x - value, y - value, z - value); }

	__forceinline vec3_t operator+(float value) const { return vec3_t(x + value, y + value, z + value); }

	__forceinline vec3_t operator/(float value) const { return vec3_t(x / value, y / value, z / value); }

	__forceinline vec3_t operator*(float value) const { return vec3_t(x * value, y * value, z * value); }

	__forceinline vec3_t operator-() const { return vec3_t(-x, -y, -z); }

	__forceinline vec3_t& operator-=(const vec3_t& value) {
		x -= value.x;
		y -= value.y;
		z -= value.z;

		return *this;
	}

	__forceinline vec3_t& operator+=(const vec3_t& value) {
		x += value.x;
		y += value.y;
		z += value.z;

		return *this;
	}

	__forceinline vec3_t& operator/=(const vec3_t& value) {
		x /= value.x;
		y /= value.y;
		z /= value.z;

		return *this;
	}

	__forceinline vec3_t& operator*=(const vec3_t& value) {
		x *= value.x;
		y *= value.y;
		z *= value.z;

		return *this;
	}

	__forceinline vec3_t& operator/=(float value) {
		x /= value;
		y /= value;
		z /= value;

		return *this;
	}

	__forceinline vec3_t& operator*=(float value) {
		x *= value;
		y *= value;
		z *= value;

		return *this;
	}

	__forceinline vec3_t& normalize() {
		auto ret = vec3_t();
		const auto len = length();
		
		if (len != 0.f) {
			ret /= len;
		} else {
			ret.x = ret.y = 0.f; ret.z = 1.f;
		}

		return ret;
	}

	__forceinline vec3_t normalized() const {
		auto ret = *this;

		return ret.normalize();
	}

	__forceinline bool operator==(const vec3_t& value) const { return x == value.x && y == value.y && z == value.z; }

	__forceinline bool operator!=(const vec3_t& value) const { return !(operator==(value)); }

	__forceinline float& operator[](int i) { return reinterpret_cast<float*>(this)[i]; }

	__forceinline const float operator[](int i) const { return reinterpret_cast<const float*>(this)[i]; }

	__forceinline float length_sqr() const { return x * x + y * y + z * z; }

	__forceinline float length() const { return math::sqrt(length_sqr()); }

	__forceinline float length_2d() const { return math::sqrt(x * x + y * y); }

	__forceinline float dot_product(const vec3_t& value) const { return x * value.x + y * value.y + z * value.z; }

	__forceinline float dot_product(const float* value) const { return x * value[0] + y * value[1] + z * value[2]; }

	__forceinline vec3_t cross_product(const vec3_t& value) const { return vec3_t(y * value.z - z * value.y, z * value.x - x * value.z, x * value.y - y * value.x); }

	vec3_t transform(const matrix3x4_t& value) const;

	vec3_t i_transform(const matrix3x4_t& value) const;

	vec3_t rotate(const matrix3x4_t& value) const;

	vec3_t i_rotate(const matrix3x4_t& value) const;

	qangle_t angle() const;

	qangle_t angle(const vec3_t& up) const;

	__forceinline float dist_to(const vec3_t& value) const { return (*this - value).length(); }

	__forceinline float dist_to_2d(const vec3_t& value) const { return (*this - value).length_2d(); }

	__forceinline bool is_valid() const { return std::isfinite(x) && std::isfinite(y) && std::isfinite(z); }

	__forceinline bool empty() const { return x == 0.f && y == 0.f && z == 0.f; }
};

struct vec4_t : public vec3_t {
	vec4_t() = default;
	vec4_t(float ix, float iy, float iz, float iw) {
		x = ix;
		y = iy;
		z = iz;
		w = iw;
	}
	vec4_t(const vec3_t& value) {
		x = value.x;
		y = value.y;
		z = value.z;
		w = 0.f;
	}
	vec4_t(const vec2_t& value0, const vec2_t& value1) {
		x = value0.x;
		y = value0.y;
		z = value1.x;
		w = value1.y;
	}

	__forceinline vec4_t& operator=(const vec3_t& value) {
		x = value.x;
		y = value.y;
		z = value.z;
		w = 0.f;

		return *this;
	}

	__forceinline vec4_t& operator=(const vec4_t& value) {
		x = value.x;
		y = value.y;
		z = value.z;
		w = value.w;

		return *this;
	}

	float w = 0.f;
};

struct ALIGN16 vec4a_t : public vec3_t {
	vec4a_t() = default;
	vec4a_t(float ix, float iy, float iz) {
		x = ix;
		y = iy;
		z = iz;
	}

	vec4a_t(const vec3_t& value) {
		x = value.x;
		y = value.y;
		z = value.z;
	}

	__forceinline vec4a_t& operator=(const vec3_t& value) {
		x = value.x;
		y = value.y;
		z = value.z;
		w = 0.f;

		return *this;
	}

	__forceinline vec4a_t& operator=(const vec4a_t& value) {
		x = value.x;
		y = value.y;
		z = value.z;
		w = value.w;

		return *this;
	}

	float w = 0.f;
};

struct qangle_t {
	qangle_t() = default;
	qangle_t(float ix, float iy, float iz) {
		x = ix;
		y = iy;
		z = iz;
	}

	float x = 0.f, y = 0.f, z = 0.f;

	__forceinline qangle_t operator+(const qangle_t& value) const { return qangle_t(x + value.x, y + value.y, z + value.z); }

	__forceinline qangle_t operator-(const qangle_t& value) const { return qangle_t(x - value.x, y - value.y, z - value.z); }

	__forceinline qangle_t operator-(float value) const { return qangle_t(x - value, y - value, z - value); }

	__forceinline qangle_t operator+(float value) const { return qangle_t(x + value, y + value, z + value); }

	__forceinline qangle_t operator/(float value) const { return qangle_t(x / value, y / value, z / value); }

	__forceinline qangle_t operator*(float value) const { return qangle_t(x * value, y * value, z * value); }

	__forceinline qangle_t& operator-=(const qangle_t& value) {
		x -= value.x;
		y -= value.y;
		z -= value.z;

		return *this;
	}

	__forceinline qangle_t& operator+=(const qangle_t& value) {
		x += value.x;
		y += value.y;
		z += value.z;

		return *this;
	}

	__forceinline qangle_t& operator/=(float value) {
		x /= value;
		y /= value;
		z /= value;

		return *this;
	}

	__forceinline qangle_t& operator*=(float value) {
		x *= value;
		y *= value;
		z *= value;

		return *this;
	}

	__forceinline bool operator==(const qangle_t& value) const { return x == value.x && y == value.y && z == value.z; }

	__forceinline bool operator!=(const qangle_t& value) const { return !(operator==(value)); }

	qangle_t& sanitize();

	qangle_t& normalize() {
		x = remainderf(x, 360.f);
		y = remainderf(y, 360.f);
		z = remainderf(z, 360.f);

		return *this;
	}

	__forceinline qangle_t normalized() const {
		auto ret = *this;

		return ret.normalize();
	}

	matrix3x4_t matrix() const;

	__forceinline float length_sqr() const { return x * x + y * y + z * z; }

	__forceinline float length() const { return math::sqrt(length_sqr()); }

	__forceinline float length_2d() const { return math::sqrt(x * x + y * y); }

	__forceinline bool is_valid() const { return std::isfinite(x) && std::isfinite(y) && std::isfinite(z); }

	__forceinline bool empty() const { return x == 0.f && y == 0.f && z == 0.f; }
};

struct col_t {
	col_t() = default;
	col_t(int r, int g, int b) { set(r, g, b, 255); }
	col_t(int r, int g, int b, int a) { set(r, g, b, a); }
	col_t(const col_t& col, int a) { set(col.r(), col.g(), col.b(), a); }

	std::array<uint8_t, 4> clr = {};

	__forceinline void set(uint8_t r, uint8_t g, uint8_t b, uint8_t a) { clr = { r, g, b, a }; }

	__forceinline int r() const { return clr.at(0); }

	__forceinline int g() const { return clr.at(1); }

	__forceinline int b() const { return clr.at(2); }

	__forceinline int a() const { return clr.at(3); }

	__forceinline unsigned int hex(bool rgba = false) const {
		return rgba
			? ((r() & 0xFF) << 24) + ((g() & 0xFF) << 16) + ((b() & 0xFF) << 8) + (a() & 0xFF)
			: ((a() & 0xFF) << 24) + ((b() & 0xFF) << 16) + ((g() & 0xFF) << 8) + (r() & 0xFF);
	}

	__forceinline col_t& alpha(int value) { return col_t(r(), g(), b(), value); }

	__forceinline col_t& operator=(const col_t& value) {
		set(value.r(), value.g(), value.b(), value.a());

		return *this;
	}

	__forceinline col_t& operator-=(uint8_t value) {
		set(r() - value, g() - value, b() - value, a() - value);

		return *this;
	}

	__forceinline col_t& operator+=(uint8_t value) {
		set(r() + value, g() + value, b() + value, a() + value);

		return *this;
	}

	__forceinline col_t& operator/=(uint8_t value) {
		set(r() / value, g() / value, b() / value, a() / value);

		return *this;
	}

	__forceinline col_t& operator*=(uint8_t value) {
		set(r() * value, g() * value, b() * value, a() * value);

		return *this;
	}

	__forceinline col_t& operator-=(const col_t& value) {
		set(r() - value.r(), g() - value.g(), b() - value.b(), a() - value.a());

		return *this;
	}

	__forceinline col_t& operator+=(const col_t& value) {
		set(r() + value.r(), g() + value.g(), b() + value.b(), a() + value.a());

		return *this;
	}

	__forceinline col_t operator-(uint8_t value) const { return col_t(r() - value, g() - value, b() - value, a() - value); }

	__forceinline col_t operator+(uint8_t value) const { return col_t(r() + value, g() + value, b() + value, a() + value); }

	__forceinline col_t operator/(uint8_t value) const { return col_t(r() / value, g() / value, b() / value, a() / value); }

	__forceinline col_t operator*(uint8_t value) const { return col_t(r() * value, g() * value, b() * value, a() * value); }

	__forceinline col_t operator-(const col_t& value) const { return col_t(r() - value.r(), g() - value.g(), b() - value.b(), a() - value.a()); }

	__forceinline col_t operator+(const col_t& value) const { return col_t(r() + value.r(), g() + value.g(), b() + value.b(), a() + value.a()); }

	__forceinline bool operator==(const col_t& value) const { return *const_cast<col_t*>(this) == *const_cast<col_t*>(&value); }

	__forceinline bool operator!=(const col_t& value) const { return !(operator==(value)); }

	__forceinline float hue() const {
		const auto red = r() / 255.f;
		const auto green = g() / 255.f;
		const auto blue = b() / 255.f;

		const auto max = max(red, max(green, blue));
		const auto min = min(red, min(green, blue));

		if (max == min)
			return 0.f;

		const auto delta = max - min;

		auto hue = 0.f;

		if (max == red) {
			hue = (green - blue) / delta;
		}
		else if (max == green) {
			hue = 2.f + (blue - red) / delta;
		}
		else {
			hue = 4.f + (red - green) / delta;
		}

		hue *= 60.f;

		if (hue < 0.f) {
			hue += 360.f;
		}

		return hue / 360.f;
	}

	__forceinline float saturation() const {
		const auto red = r() / 255.f;
		const auto green = g() / 255.f;
		const auto blue = b() / 255.f;

		const auto max = max(red, max(green, blue));
		const auto min = min(red, min(green, blue));

		const auto delta = max - min;

		if (max == 0.f)
			return delta;

		return delta / max;
	}

	__forceinline float brightness() const { return max(r() / 255.f, max(g() / 255.f, b() / 255.f)); }

	__forceinline static col_t from_hsb(float hue, float saturation, float brightness) {
		const auto h = hue == 1.f ? 0 : hue * 6.f;
		const auto f = h - static_cast<int>(h);
		const auto p = brightness * (1.f - saturation);
		const auto q = brightness * (1.f - saturation * f);
		const auto t = brightness * (1.f - (saturation * (1.f - f)));

		if (h < 1.f) {
			return col_t(
				static_cast<uint8_t>(brightness * 255.f),
				static_cast<uint8_t>(t * 255.f),
				static_cast<uint8_t>(p * 255.f)
			);
		}

		if (h < 2.f) {
			return col_t(
				static_cast<uint8_t>(q * 255),
				static_cast<uint8_t>(brightness * 255),
				static_cast<uint8_t>(p * 255)
			);
		}

		if (h < 3.f) {
			return col_t(
				static_cast<uint8_t>(p * 255.f),
				static_cast<uint8_t>(brightness * 255.f),
				static_cast<uint8_t>(t * 255.f)
			);
		}

		if (h < 4.f) {
			return col_t(
				static_cast<uint8_t>(p * 255.f),
				static_cast<uint8_t>(q * 255.f),
				static_cast<uint8_t>(brightness * 255.f)
			);
		}

		if (h < 5.f) {
			return col_t(
				static_cast<uint8_t>(t * 255.f),
				static_cast<uint8_t>(p * 255.f),
				static_cast<uint8_t>(brightness * 255.f)
			);
		}

		return col_t(
			static_cast<uint8_t>(brightness * 255.f),
			static_cast<uint8_t>(p * 255.f),
			static_cast<uint8_t>(q * 255.f)
		);
	}

	struct palette_t {
		static col_t red(int alpha = 255) { return col_t(255, 0, 0, alpha); }
		static col_t green(int alpha = 255) { return col_t(0, 255, 0, alpha); }
		static col_t blue(int alpha = 255) { return col_t(0, 0, 255, alpha); }

		static col_t white(int alpha = 255) { return col_t(255, 255, 255, alpha); }
		static col_t black(int alpha = 255) { return col_t(13, 13, 13, alpha); }
		static col_t grey(int alpha = 255) { return col_t(60, 60, 60, alpha); }
		static col_t orange(int alpha = 255) { return col_t(245, 171, 53, alpha); }

		static col_t light_black(int alpha = 255) { return col_t(32, 32, 32, alpha); }
		static col_t light_grey(int alpha = 255) { return col_t(77, 77, 77, alpha); }
		static col_t light_green(int alpha = 255) { return col_t(127, 255, 0, alpha); }
		static col_t light_red(int alpha = 255) { return col_t(236, 98, 95, alpha); }
		static col_t light_blue(int alpha = 255) { return col_t(42, 87, 233, alpha); }
		static col_t light_violet(int alpha = 255) { return col_t(153, 153, 255, alpha); }
		static col_t light_purple(int alpha = 255) { return col_t(127, 73, 208, alpha); }

		static col_t dark_grey(int alpha = 255) { return col_t(44, 44, 44, alpha); }
		static col_t dark_blue(int alpha = 255) { return col_t(14, 18, 134, alpha); }

		static col_t purple(int alpha = 255) { return col_t(220, 0, 220, alpha); }
		static col_t pink(int alpha = 255) { return col_t(255, 105, 180, alpha); }
	};
};

struct matrix3x4_t {
	matrix3x4_t() = default;
	matrix3x4_t(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23) {
		m_matrix[0][0] = m00;
		m_matrix[0][1] = m01;
		m_matrix[0][2] = m02;
		m_matrix[0][3] = m03;

		m_matrix[1][0] = m10;
		m_matrix[1][1] = m11;
		m_matrix[1][2] = m12;
		m_matrix[1][3] = m13;

		m_matrix[2][0] = m20;
		m_matrix[2][1] = m21;
		m_matrix[2][2] = m22;
		m_matrix[2][3] = m23;
	}

	matrix3x4_t(const vec3_t& x_axis, const vec3_t& y_axis, const vec3_t& z_axis, const vec3_t& origin) {
		m_matrix[0][0] = x_axis.x;
		m_matrix[0][1] = y_axis.x;
		m_matrix[0][2] = z_axis.x;
		m_matrix[0][3] = origin.x;

		m_matrix[1][0] = x_axis.y;
		m_matrix[1][1] = y_axis.y;
		m_matrix[1][2] = z_axis.y;
		m_matrix[1][3] = origin.y;

		m_matrix[2][0] = x_axis.z;
		m_matrix[2][1] = y_axis.z;
		m_matrix[2][2] = z_axis.z;
		m_matrix[2][3] = origin.z;
	}

	__forceinline vec3_t get_column(int column) const { return vec3_t(m_matrix[0][column], m_matrix[1][column], m_matrix[2][column]); }

	__forceinline void set_column(const vec3_t& value, int column) {
		m_matrix[0][column] = value.x;
		m_matrix[1][column] = value.y;
		m_matrix[2][column] = value.z;
	}

	__forceinline void add_to_column(const vec3_t& value, int column) {
		m_matrix[0][column] += value.x;
		m_matrix[1][column] += value.y;
		m_matrix[2][column] += value.z;
	}

	__forceinline matrix3x4_t operator+(const matrix3x4_t& value) const {
		return matrix3x4_t(
			m_matrix[0][0] + value[0][0], m_matrix[0][1] + value[0][1], m_matrix[0][2] + value[0][2], m_matrix[0][3] + value[0][3],
			m_matrix[1][0] + value[1][0], m_matrix[1][1] + value[1][1], m_matrix[1][2] + value[1][2], m_matrix[1][3] + value[1][3],
			m_matrix[2][0] + value[2][0], m_matrix[2][1] + value[2][1], m_matrix[2][2] + value[2][2], m_matrix[2][3] + value[2][3]
		);
	}

	__forceinline matrix3x4_t operator*(const matrix3x4_t& value) const {
		return matrix3x4_t(
			m_matrix[0][0] * value[0][0] + m_matrix[0][1] * value[1][0] + m_matrix[0][2] * value[2][0],
			m_matrix[0][0] * value[0][1] + m_matrix[0][1] * value[1][1] + m_matrix[0][2] * value[2][1],
			m_matrix[0][0] * value[0][2] + m_matrix[0][1] * value[1][2] + m_matrix[0][2] * value[2][2],
			m_matrix[0][0] * value[0][3] + m_matrix[0][1] * value[1][3] + m_matrix[0][2] * value[2][3] + m_matrix[0][3],
			m_matrix[1][0] * value[0][0] + m_matrix[1][1] * value[1][0] + m_matrix[1][2] * value[2][0],
			m_matrix[1][0] * value[0][1] + m_matrix[1][1] * value[1][1] + m_matrix[1][2] * value[2][1],
			m_matrix[1][0] * value[0][2] + m_matrix[1][1] * value[1][2] + m_matrix[1][2] * value[2][2],
			m_matrix[1][0] * value[0][3] + m_matrix[1][1] * value[1][3] + m_matrix[1][2] * value[2][3] + m_matrix[1][3],
			m_matrix[2][0] * value[0][0] + m_matrix[2][1] * value[1][0] + m_matrix[2][2] * value[2][0],
			m_matrix[2][0] * value[0][1] + m_matrix[2][1] * value[1][1] + m_matrix[2][2] * value[2][1],
			m_matrix[2][0] * value[0][2] + m_matrix[2][1] * value[1][2] + m_matrix[2][2] * value[2][2],
			m_matrix[2][0] * value[0][3] + m_matrix[2][1] * value[1][3] + m_matrix[2][2] * value[2][3] + m_matrix[2][3]
		);
	}

	__forceinline matrix3x4_t operator*(float value) const {
		return matrix3x4_t(
			m_matrix[0][0] * value,
			m_matrix[0][1] * value,
			m_matrix[0][2] * value,
			m_matrix[0][3] * value,
			m_matrix[1][0] * value,
			m_matrix[1][1] * value,
			m_matrix[1][2] * value,
			m_matrix[1][3] * value,
			m_matrix[2][0] * value,
			m_matrix[2][1] * value,
			m_matrix[2][2] * value,
			m_matrix[2][3] * value
		);
	}

	__forceinline vec3_t operator*(const vec3_t& value) const {
		return vec3_t(
			m_matrix[0][0] * value.x + m_matrix[0][1] * value.y + m_matrix[0][2] * value.z + m_matrix[0][3],
			m_matrix[1][0] * value.x + m_matrix[1][1] * value.y + m_matrix[1][2] * value.z + m_matrix[1][3],
			m_matrix[2][0] * value.x + m_matrix[2][1] * value.y + m_matrix[2][2] * value.z + m_matrix[2][3]
		);
	}

	qangle_t angle() const;

	__forceinline vec4_t& operator[](int i) { return m_matrix.at(i); }

	__forceinline const vec4_t& operator[](int i) const { return m_matrix.at(i); }

	std::array<vec4_t, 3> m_matrix = {};
};

struct ALIGN16 matrix3x4a_t : public matrix3x4_t {

};

struct v_matrix {
	__forceinline vec4_t& operator[](int i) { return m_matrix.at(i); }

	__forceinline const vec4_t& operator[](int i) const { return m_matrix.at(i); }

	std::array<vec4_t, 4> m_matrix = {};
};

struct bit_flag_t {
	bit_flag_t() = default;
	bit_flag_t(uint32_t value) { m_value = value; }

	__forceinline bool has(uint32_t value) const { return m_value & value; }

	__forceinline void add(uint32_t value) { m_value |= value; }

	__forceinline void remove(uint32_t value) { m_value &= ~value; }

	__forceinline bool is_valid() const { return m_value; }

	__forceinline operator uint32_t() const { return m_value; }

	__forceinline bit_flag_t& operator=(bit_flag_t value) {
		m_value = value.m_value;

		return *this;
	}

	__forceinline uint32_t& operator=(uint32_t value) {
		m_value = value;

		return m_value;
	}

	uint32_t m_value = 0u;
};

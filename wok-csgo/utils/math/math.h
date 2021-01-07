#pragma once

namespace math {
	constexpr auto m_pi = 3.14159265358979323846f;
	constexpr auto m_rad_pi = 180.f / m_pi;
	constexpr auto m_deg_pi = m_pi / 180.f;
	constexpr auto m_round_error = std::numeric_limits<double>::round_error();

	void sin_cos(float rad, float& sin, float& cos);

	__forceinline float clamp(float value, float min, float max) {
		_mm_store_ss(&value, _mm_min_ss(_mm_max_ss(_mm_set_ss(value), _mm_set_ss(min)), _mm_set_ss(max)));
		return value;
	}

	template<typename T>
	__forceinline T rad_to_deg(const T& value) {
		return value * static_cast<T>(m_rad_pi);
	}

	template<typename T>
	__forceinline T deg_to_rad(const T& value) {
		return value * static_cast<T>(m_deg_pi);
	}

	template<typename A, typename B>
	__forceinline auto min(const A& a, const B& b) -> decltype(a < b ? a : b) {
		return a < b ? a : b;
	}

	template<typename A, typename B>
	__forceinline auto max(const A& a, const B& b) -> decltype(a > b ? a : b) {
		return a > b ? a : b;
	}

	template<typename T, typename L, typename H>
	__forceinline T clamp(const T& value, const L& min, const H& max) {
		return (value < min) ? min : (value > max) ? max : value;
	}

	double __forceinline __declspec (naked) __fastcall sin(double x) {
		__asm {
			fld	qword ptr [esp + 4]
			fsin
			ret	8
		}
	}

	double __forceinline __declspec (naked) __fastcall cos(double x) {
		__asm {
			fld	qword ptr [esp + 4]
			fcos
			ret	8
		}
	}

	double __forceinline __declspec (naked) __fastcall atan2(double y, double x) {
		__asm {
			fld	qword ptr [esp + 4]
			fld	qword ptr [esp + 12]
			fpatan
			ret	16
		}
	}

	double __forceinline __declspec (naked) __fastcall atan(double x) {
		__asm {
			fld	qword ptr [esp + 4]
			fld1
			fpatan
			ret	8
		}
	}

	double __forceinline __declspec (naked) __fastcall asin(double x) {
		__asm {
			fld	qword ptr [esp + 4]
			fld	st
			fabs
			fcom dword ptr [m_round_error]
			fstsw ax
			sahf
			jbe skip

			fld1
			fsubrp st(1), st(0)
			fld	st
			fadd st(0), st(0)
			fxch st(1)
			fmul st(0), st(0)
			fsubp st(1), st(0)
			jmp	end

			skip:
			fstp st(0)
			fld	st(0)
			fmul st(0), st(0)
			fld1
			fsubrp st(1), st(0)

			end:
			fsqrt
			fpatan
			ret	8
		}
	}

	double __forceinline __declspec (naked) __fastcall acos(double x) {
		__asm {
			fld	qword ptr[esp + 4]
			fld1
			fchs
			fcomp st(1)
			fstsw ax
			je skip

			fld	st(0)
			fld1
			fsubrp st(1), st(0)
			fxch st(1)
			fld1
			faddp st(1), st(0)
			fdivp st(1), st(0)
			fsqrt
			fld1
			jmp	end

			skip:
			fld1
			fldz

			end:
			fpatan
			fadd st(0), st(0)
			ret	8
		}
	}
	
	double __forceinline __declspec (naked) __fastcall floor(double x) {
		__asm {
			fld	qword ptr [esp + 4]
			fld1
			fld	st(1)
			fprem
			sub	esp, 4
			fst	dword ptr [esp]
			fxch st(2)
			mov	eax, [esp]
			cmp eax, 80000000h
			jbe end
			fsub st, st(1)

			end:
			fsub st, st(2)
			fstp st(1)
			fstp st(1)
			pop	eax
			ret	8
		}
	}

	void angle_vectors(const qangle_t& angles, vec3_t* forward, vec3_t* right = nullptr, vec3_t* up = nullptr);

	qangle_t calc_angle(const vec3_t& src, const vec3_t& dst);
}

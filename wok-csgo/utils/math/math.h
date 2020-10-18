#pragma once

namespace math {
	constexpr auto m_pi = 3.14159265358979323846f;
	constexpr auto m_rad_pi = 180.f / m_pi;
	constexpr auto m_one2 = 0.5;
	void sin_cos(float rad, float& sin, float& cos);

	__forceinline float rad_to_deg(float rad) { return rad * m_rad_pi; }

	__forceinline float deg_to_rad(float deg) { return deg * (m_pi / 180.f); }

	__forceinline float clamp(float value, float min, float max) {
		_mm_store_ss(&value, _mm_min_ss(_mm_max_ss(_mm_set_ss(value), _mm_set_ss(min)), _mm_set_ss(max)));
		return value;
	}

	double __forceinline __declspec (naked) __fastcall asin(double x) {
		__asm {
			fld		qword ptr [esp + 4]
			fld		st
			fabs
			fcom		dword ptr [m_one2]
			fstsw		ax
			sahf
			jbe		asin_cont
			fld1
			fsubrp		st(1), st(0)
			fld		st
			fadd		st(0), st(0)
			fxch		st(1)
			fmul		st(0), st(0)
			fsubp		st(1), st(0)
			jmp		asin_exit

			asin_cont:

			fstp		st(0)
			fld		st(0)
			fmul		st(0), st(0)
			fld1
			fsubrp		st(1), st(0)

			asin_exit:

			fsqrt
			fpatan
			ret		8
		}
	}
	
	double __forceinline __declspec (naked) __fastcall tan(double x) {
		__asm {
			fld		qword ptr [esp + 4]
			fptan
			fstp		st(0)
			ret		8
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

	double __forceinline __declspec (naked) __fastcall acos(double x) {
		__asm {
			fld	qword ptr [esp + 4]
			fld1
			fchs
			fcomp st(1)
			fstsw ax
			je aaaaaaaa

			fld	st(0)
			fld1
			fsubrp st(1), st(0)
			fxch st(1)
			fld1
			faddp st(1), st(0)
			fdivp st(1), st(0)
			fsqrt
			fld1
			jmp	finish

			aaaaaaaa:
			fld1
			fldz

			finish:
			fpatan
			fadd st(0), st(0)
			ret	8
		}
	}
	
	double __forceinline __declspec (naked) __fastcall floor(double x) {
		__asm {
			fld		qword ptr [esp + 4]
			fld1
			fld		st(1)
			fprem
			sub		esp, 4
			fst		dword ptr [esp]
			fxch		st(2)
			mov		eax, [esp]
			cmp		eax, 0x80000000
			jbe		floor_exit
			fsub		st, st(1)

			floor_exit:

			fsub		st, st(2)
			fstp		st(1)
			fstp		st(1)
			pop		eax
			ret		8
		}
	}

	void angle_vectors(const qangle_t& angles, vec3_t* forward, vec3_t* right = nullptr, vec3_t* up = nullptr);

	qangle_t calc_angle(const vec3_t& src, const vec3_t& dst);
}

#pragma once

namespace math {
	void sin_cos(float rad, float& sin, float& cos);

	__forceinline float clamp(float value, float min, float max) {
		_mm_store_ss(&value, _mm_min_ss(_mm_max_ss(_mm_set_ss(value), _mm_set_ss(min)), _mm_set_ss(max)));
		return value;
	}

	void angle_vectors(const qangle_t& angles, vec3_t* forward, vec3_t* right = nullptr, vec3_t* up = nullptr);

	qangle_t calc_angle(const vec3_t& src, const vec3_t& dst);

	/// <summary>
	/// Fast inverse operation of sin of x.
	/// </summary>
	/// <param name="x">Value whose arc sine is computed</param>
	/// <returns>Principal arc sine of x</returns>
	double __forceinline __declspec (naked) __fastcall asin(double x) {
		__asm {
			fld		qword ptr [esp + 4]
			fld		st
			fmul		st, st
			fld1
			faddp		st(1), st
			fsqrt
			faddp		st(1), st
			fldln2
			fxch
			fyl2x
			ret		8
		}
	}

	/// <summary>
	/// Fast arc tangent of y/x.
	/// </summary>
	/// <param name="y">Value representing the proportion of the y-coordinate.</param>
	/// <param name="x">Value representing the proportion of the x-coordinate.</param>
	/// <returns>Principal value of the arc tangent of y/x.</returns>
	double __forceinline __declspec (naked) __fastcall atan(double y, double x) {
		__asm {
			fld		qword ptr [esp + 4]
			fld		qword ptr [esp + 12]
			fpatan
			ret		16
		}
	}

	/// <summary>
	/// Fast arc tangent of x.
	/// </summary>
	/// <param name="x">Value whose arc tangent is computed.</param>
	/// <returns>Principal arc tangent of x.</returns>
	double __forceinline __declspec (naked) __fastcall atan(double x) {
		__asm {
			fld		qword ptr [esp + 4] 
			fld1 
			fpatan 
			ret		8 
		}
	}

	/// <summary>
	/// Fast sine of an angle of x radians.
	/// </summary>
	/// <param name="x">Value representing an angle expressed in radians.</param>
	/// <returns>Sine of x radians.</returns>
	double __forceinline __declspec (naked) __fastcall sin(double x) {
		__asm {
			fld		qword ptr [esp + 4]
			fsin		
			ret		8
		}
	}

	/// <summary>
	/// Fast cosine of an angle of x radians.
	/// </summary>
	/// <param name="x">Value representing an angle expressed in radians.</param>
	/// <returns>Cosine of x radians.</returns>
	double __forceinline __declspec (naked) __fastcall cos(double x) {
		__asm {
			fld		qword ptr [esp + 4]
			fcos
			ret		8
		}
	}

	/// <summary>
	/// Fast principal value of the arc cosine of x, expressed in radians.
	/// </summary>
	/// <param name="x">Value whose arc cosine is computed.</param>
	/// <returns>Principal arc cosine of x.</returns>
	double __forceinline __declspec (naked) __fastcall acos(double x) {
		__asm {
			fld		qword ptr [esp + 4] 
			fld1 
			fchs 
			fcomp		st(1) 
			fstsw		ax 
			je			acos_po_pizde

			fld		st(0) 
			fld1 
			fsubrp	st(1), st(0) 
			fxch		st(1) 
			fld1 
			faddp		st(1), st(0) 
			fdivp		st(1), st(0) 
			fsqrt 
			fld1 
			jmp		acos_exit 

			acos_po_pizde: 

			fld1 
			fldz 

			acos_exit: 

			fpatan 
			fadd		st(0), st(0) 
			ret		8 
		}
	}
}

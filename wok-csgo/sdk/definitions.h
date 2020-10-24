#pragma once

#define IN_RANGE(a, b, c)		(a >= b && a <= c) 

#define TICK_INTERVAL			(interfaces::global_vars->m_interval_per_tick)

#define TIME_TO_TICKS(dt)		(static_cast<int>(0.5f + static_cast<float>(dt) / TICK_INTERVAL))
#define TICKS_TO_TIME(t)		(TICK_INTERVAL * (t))
#define ROUND_TO_TICKS(t)		(TICK_INTERVAL * TIME_TO_TICKS(t))

#define MAX_COORD_FLOAT (16384.f)
#define MIN_COORD_FLOAT (-MAX_COORD_FLOAT)

#define MULTIPLAYER_BACKUP				150

#define MAX_PENETRATION_DISTANCE	90.f

#define MAX_OS_PATH 260

#define CAM_HULL_OFFSET		14.f
#define CAM_HULL_MIN		vec3_t(-CAM_HULL_OFFSET, -CAM_HULL_OFFSET, -CAM_HULL_OFFSET)
#define CAM_HULL_MAX		vec3_t(CAM_HULL_OFFSET, CAM_HULL_OFFSET, CAM_HULL_OFFSET)

struct anim_layer_t;

using pose_params_t = std::array<float, 24u>;
using anim_layers_t = std::array<anim_layer_t, 15u>;

namespace math {
	double __forceinline __declspec (naked) __fastcall sqrt(double n) {
		_asm {
			fld qword ptr [esp + 4]
			fsqrt
			ret 8
		}
	}
}

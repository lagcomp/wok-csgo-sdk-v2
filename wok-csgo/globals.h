#pragma once
#include "common_includes.h"
#include "sdk/interfaces.h"
#include "singleton.h"
#include "menu/menu.h"

namespace g {
	namespace angles {
		inline qangle_t		view;
		inline qangle_t		real;
	}

	inline HMODULE			module;
	inline c_local_player	local;
	inline c_user_cmd*		cmd;
	inline bool				send_packet;
}

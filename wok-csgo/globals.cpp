#include "globals.h"

namespace g {
	namespace angles {
		angle_t    view = {};
		angle_t    real = {};
	}

	HMODULE			module = nullptr;
	c_local_player	local = {};
	c_user_cmd*		cmd = nullptr;
	bool			send_packet = true;
}
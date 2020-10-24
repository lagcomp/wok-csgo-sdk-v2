#pragma once
#include "hooks/hooks.h"
#include "events/events.h"

namespace wok {
	__forceinline void init() {
		while (memory::m_modules.find(HASH("serverbrowser.dll")) == memory::m_modules.end()) {
			memory::get_all_modules();

			std::this_thread::sleep_for(std::chrono::milliseconds(200u));
		}

		g::local = *SIG("client.dll", "8B 0D ? ? ? ? 83 FF FF 74 07").self_offset(0x2).cast<c_local_player*>();

		interfaces::init();

		input::init();

		render::init();

		netvars::init();

		cfg::init();

		hooks::init();

		events::init();
	}

	__forceinline void unload() {
		events::undo();
		
		hooks::undo();

		input::undo();
	}
}

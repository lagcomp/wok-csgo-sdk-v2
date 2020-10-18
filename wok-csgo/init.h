#pragma once
#include "hooks/hooks.h"
#include "events/events.h"

namespace wok {
	void init() {
		while (memory::m_modules.find(FNV1A("serverbrowser.dll")) == memory::m_modules.end()) {
			memory::get_all_modules();

			std::this_thread::sleep_for(std::chrono::milliseconds(200u));
		}

		g::local = *SIG("client.dll", "8B 0D ? ? ? ? 83 FF FF 74 07").self_offset(0x2).cast<c_local_player*>();

		input::init();

		interfaces::init();

		render::init();

		netvars::init();

		cfg::init();

		hooks::init();

		events::init();
	}

	void unload() {
		events::undo();
		
		hooks::undo();

		input::undo();
	}
}

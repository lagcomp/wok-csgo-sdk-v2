#pragma once
#include "../globals.h"
#include "../features/features.h"

namespace events {
	class c_listener : public i_game_event_listener {
	public:
		c_listener() = default;
		~c_listener() = default;

		void fire_game_event(i_game_event* event);
		int get_event_debug_id() { return EVENT_DEBUG_ID_INIT; }
	};

	void init();

	void undo();

	void player_hurt(i_game_event* event);

	extern c_listener m_listener;

	extern std::unordered_map<uint32_t, std::function<void(i_game_event*)>> m_callbacks;
}

#define ADD_CALLBACK(name, fn) m_callbacks[FNV1A(name)] = fn; interfaces::event_manager->add_listener(&m_listener, _(name), false);

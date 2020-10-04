#include "events.h"

namespace events {
	__forceinline void c_listener::fire_game_event(i_game_event* event) { m_callbacks.at(FNV1A_RT(event->get_name()))(event); }

	__forceinline void add_callback(const char* name, std::function<void(i_event*)> fn) {
		m_callbacks[FNV1A_RT(name)] = fn;
		interfaces::event_manager->add_listener(&m_listener, _(name), false);
	}
	
	__forceinline void init() {
		add_callback("player_hurt", player_hurt);
	}

	__forceinline void undo() { interfaces::event_manager->remove_listener(&m_listener); }

	c_listener m_listener;

	std::unordered_map<uint32_t, std::function<void(i_game_event*)>> m_callbacks;
}

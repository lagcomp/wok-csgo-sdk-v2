#include "../hooks.h"

void __fastcall hooks::panel::paint_traverse::fn(void* ecx, void* edx, uint32_t id, bool force_repaint, bool allow_force) {
	static const auto original = m_panel->get_original<T>(index);

	if (HASH_RT(interfaces::panel->get_name(id)) == HASH("MatSystemTopPanel")) {
		render::begin();
	}

	original(interfaces::panel, id, force_repaint, allow_force);
}
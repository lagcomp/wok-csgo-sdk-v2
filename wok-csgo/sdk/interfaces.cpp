#include "interfaces.h"

namespace interfaces {
	void init() {
		INTERFACE_EXPORT(client_dll, i_base_client_dll, "client.dll", "VClient018")
		INTERFACE_EXPORT(engine, i_engine_client, "engine.dll", "VEngineClient014")
		INTERFACE_EXPORT(entity_list, i_client_entity_list, "client.dll", "VClientEntityList003")

		INTERFACE_OFFSET(client_mode, i_client_mode, client_dll, 10, 0x5)
		INTERFACE_OFFSET(global_vars, i_global_vars, client_dll, 11, 0xA)
		INTERFACE_OFFSET(client_state, i_client_state, engine, 12, 0x10)

		mem_alloc = *EXPORT("tier0.dll", "g_pMemAlloc").cast<i_mem_alloc**>();

		INTERFACE_EXPORT(model_info, i_model_info, "engine.dll", "VModelInfoClient004")
		INTERFACE_EXPORT(surface, i_surface, "vguimatsurface.dll", "VGUI_Surface031")

		INTERFACE_SIG(input, i_input, "client.dll", "B9 ? ? ? ? 8B 40 38 FF D0 84 C0 0F 85", 0x1)

		PINTERFACE_SIG(move_helper, i_move_helper, "client.dll", "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01", 0x2)

		INTERFACE_EXPORT(prediction, i_prediction, "client.dll", "VClientPrediction001")
		INTERFACE_EXPORT(game_movement, i_game_movement, "client.dll", "GameMovement001")
		INTERFACE_EXPORT(debug_overlay, i_debug_overlay, "engine.dll", "VDebugOverlay004")
		INTERFACE_EXPORT(cvar_system, i_cvar_system, "vstdlib.dll", "VEngineCvar007")
		INTERFACE_EXPORT(event_manager, i_game_event_manager, "engine.dll", "GAMEEVENTSMANAGER002")
		INTERFACE_EXPORT(trace_system, i_engine_trace, "engine.dll", "EngineTraceClient004")
		INTERFACE_EXPORT(surface_data, i_surface_data, "vphysics.dll", "VPhysicsSurfaceProps001")
		INTERFACE_EXPORT(input_system, i_input_system, "inputsystem.dll", "InputSystemVersion001")
		INTERFACE_EXPORT(leaf_system, i_client_leaf_system, "client.dll", "ClientLeafSystem002")

		INTERFACE_SIG(game_rules, i_game_rules*, "client.dll", "A1 ? ? ? ? 85 C0 0F 84 ? ? ? ? 80 B8 ? ? ? ? ? 74 7A", 0x1)

		INTERFACE_EXPORT(game_types, i_game_types, "matchmaking.dll", "VENGINE_GAMETYPES_VERSION")
		INTERFACE_EXPORT(model_cache, i_mdl_cache, "datacache.dll", "MDLCache004")
		INTERFACE_EXPORT(model_render, i_model_render, "engine.dll", "VEngineModel016")
		INTERFACE_EXPORT(material_system, i_material_system, "materialsystem.dll", "VMaterialSystem080")

		INTERFACE_SIG(glow_manager, i_glow_object_manager, "client.dll", "0F 11 05 ? ? ? ? 83 C8 01", 0x3)
		INTERFACE_SIG(beams, i_view_render_beams, "client.dll", "B9 ? ? ? ? A1 ? ? ? ? FF 10 A1 ? ? ? ? B9", 0x1)
		INTERFACE_SIG(weapon_system, i_weapon_system, "client.dll", "8B 35 ? ? ? ? FF 10 0F B7 C0", 0x2)
		INTERFACE_SIG(player_resource, i_cs_player_resource*, "client.dll", "8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7", 0x2)

		INTERFACE_EXPORT(localize, i_localize, "localize.dll", "Localize_001")
		INTERFACE_EXPORT(panel, i_panel, "vgui2.dll", "VGUI_Panel009")

		uintptr_t** shader_device = nullptr;
		INTERFACE_EXPORT(shader_device, uintptr_t*, "shaderapidx9.dll", "ShaderDevice001")
		if (auto device_table = shader_device[0]) {
			if (auto d3d = device_table[37]) {
				d3d_device = **reinterpret_cast<IDirect3DDevice9***>(d3d + 0x2);
			}
		}
	}

	i_base_client_dll*		client_dll = nullptr;
	i_engine_client*		engine = nullptr;
	i_client_entity_list*	entity_list = nullptr;
	i_client_mode*			client_mode = nullptr;
	i_global_vars*			global_vars = nullptr;
	i_client_state*			client_state = nullptr;
	i_mem_alloc*			mem_alloc = nullptr;
	i_model_info*			model_info = nullptr;
	i_surface*				surface = nullptr;
	i_input*				input = nullptr;
	i_move_helper*			move_helper = nullptr;
	i_prediction*			prediction = nullptr;
	i_game_movement*		game_movement = nullptr;
	i_debug_overlay*		debug_overlay = nullptr;
	i_cvar_system*			cvar_system = nullptr;
	i_game_event_manager*	event_manager = nullptr;
	i_engine_trace*			trace_system = nullptr;
	i_surface_data*			surface_data = nullptr;
	i_input_system*			input_system = nullptr;
	i_client_leaf_system*	leaf_system = nullptr;
	i_game_rules**			game_rules = nullptr;
	i_game_types*			game_types = nullptr;
	i_mdl_cache*			model_cache = nullptr;
	i_model_render*			model_render = nullptr;
	i_material_system*		material_system = nullptr;
	i_glow_object_manager*	glow_manager = nullptr;
	i_view_render_beams*	beams = nullptr;
	i_weapon_system*		weapon_system = nullptr;
	i_cs_player_resource**	player_resource = nullptr;
	i_localize*				localize = nullptr;
	i_panel*				panel = nullptr;

	IDirect3DDevice9*		d3d_device = nullptr;
}

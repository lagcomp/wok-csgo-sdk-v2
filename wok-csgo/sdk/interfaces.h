#pragma once
#include "sdk.h"

namespace interfaces {
	void init();

	inline i_base_client_dll*		client_dll;
	inline i_engine_client*			engine;
	inline i_client_entity_list*	entity_list;
	inline i_client_mode*			client_mode;
	inline i_global_vars*			global_vars;
	inline i_client_state*			client_state;
	inline i_mem_alloc*				mem_alloc;
	inline i_model_info*			model_info;
	inline i_surface*				surface;
	inline i_input*					input;
	inline i_move_helper*			move_helper;
	inline i_prediction*			prediction;
	inline i_game_movement*			game_movement;
	inline i_debug_overlay*			debug_overlay;
	inline i_cvar*					cvar;
	inline i_game_event_manager*	event_manager;
	inline i_engine_trace*			trace;
	inline i_surface_data*			surface_data;
	inline n i_input_system*			input_system;
	inline i_client_leaf_system*	leaf_system;
	inline c_game_rules**			game_rules;
	inline i_mdl_cache*				model_cache;
	inline i_model_render*			model_render;
	inline i_material_system*		material_system;
	inline i_glow_object_manager*	glow_manager;
	inline i_view_render_beams*		beams;
	inline i_weapon_system*			weapon_system;
	inline c_cs_player_resource**	player_resource;
	inline i_localize*				localize;
	inline i_panel*					panel;

	inline IDirect3DDevice9*		d3d_device;
}

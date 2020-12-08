#pragma once

class c_ik_context {
public:
	VFUNC_SIG(construct(), "client.dll", "53 8B D9 F6 C3 03 74 0B FF 15 ?? ?? ?? ?? 84 C0 74 01 CC C7 83 ?? ?? ?? ?? ?? ?? ?? ?? 8B CB", void(__thiscall*)(void*))

	VFUNC_SIG(destruct(), "client.dll", "56 8B F1 57 8D 8E ?? ?? ?? ?? E8 ?? ?? ?? ?? 8D 8E ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 BE ?? ?? ?? ?? ??", void(__thiscall*)(void*))
		
	VFUNC_SIG(init(c_studio_hdr* hdr, qangle_t& angles, vec3_t& origin, float time, int framecount, int mask), "client.dll", "55 8B EC 83 EC ? 8B 45 ? 56 57 8B F9 8D 8F ? ? ? ?",
		void(__thiscall*)(void*, c_studio_hdr*, qangle_t&, vec3_t&, float, int, int), hdr, angles, origin, time, framecount, mask)

	VFUNC_SIG(add_dependencies(mstudioseqdesc_t& seqdesc, int sequence, float cycle, float* pose_params, float weight), "client.dll",
		"55 8B EC 81 EC ? ? ? ? 53 56 57 8B F9 0F 28 CB F3 0F 11 4D ? 8B 8F ? ? ? ? 8B 01",
		void(__thiscall*)(void*, mstudioseqdesc_t&, int, float, float*, float), seqdesc, sequence, cycle, pose_params, weight)

	VFUNC_SIG(update_targets(vec3_t* pos, vec4_t* q, matrix3x4_t* bones, uint8_t* computed), "client.dll", "55 8B EC 83 E4 ? 81 EC ? ? ? ? 33 D2",
		void(__thiscall*)(void*, vec3_t*, void*, matrix3x4_t*, uint8_t*), pos, q, bones, computed)

	VFUNC_SIG(solve_dependencies(vec3_t* pos, vec4_t* q, matrix3x4_t* bones, uint8_t* computed), "client.dll", "55 8B EC 83 E4 ? 81 EC ? ? ? ? 8B 81 ? ? ? ? 56",
		void(__thiscall*)(void*, vec3_t*, void*, matrix3x4_t*, uint8_t*), pos, q, bones, computed)

	__forceinline void clear_targets() {
		auto target = reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + 0xD0);

		for (auto i = 0; i < *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + 0xFF0); i++) {
			*target = -9999;
			target += 85;
		}
	}
};

class c_bone_setup {
public:
	__forceinline void init_pose(vec3_t* pos, vec4_t* q, c_studio_hdr* hdr) {
		static const auto init_pose_fn = SIG("client.dll", "55 8B EC 83 EC 10 53 8B D9 89 55 F8 56 57 89 5D F4 8B 0B 89 4D F0").get();
		if (!init_pose_fn)
			return;

		__asm {
			mov	eax, this
			mov	esi, q
			mov	edx, pos
			push	dword ptr [hdr + 4]
			mov	ecx, [eax]
			push	esi
			call	init_pose_fn
			add	esp, 8
		}
	}
	
	__forceinline void calc_autoplay_sequences(vec3_t* pos, vec4_t* q, float time, c_ik_context* ik)  {
		static const auto calc_autoplay_sequences_fn = SIG("client.dll", "55 8B EC 83 EC 10 53 56 57 8B 7D 10 8B D9 F3 0F 11 5D ??").get();
		if (!calc_autoplay_sequences_fn)
			return;

		__asm {
			movss xmm3, time
			mov	eax, ik
			mov	ecx, this
			push	eax
			push	q
			push	pos
			call	calc_autoplay_sequences_fn
		}
	}
	
	__forceinline void calc_bone_adj(vec3_t* pos, vec4_t* q, float* controllers, int bone_mask) {
		static const auto calc_bone_adj_fn = SIG("client.dll", "55 8B EC 83 E4 F8 81 EC ?? ?? ?? ?? 8B C1 89 54 24 04 89 44 24 2C 56 57 8B 00").get();
		if (!calc_bone_adj_fn)
			return;

		__asm {
			mov   eax, controllers
			mov   ecx, this
			mov   edx, pos
			push  dword ptr [ecx + 4]
			mov   ecx, [ecx]
			push  eax
			push  q
			call  calc_bone_adj_fn
			add   esp, 12
		}
	}
	
	VFUNC_SIG(accumulate_pose(vec3_t* pos, vec4_t* q, int sequence, float cycle, float weight, float time, c_ik_context* ik),
		"client.dll", "55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? A1", void(__thiscall*)(void*, vec3_t*, vec4_t*, int, float, float, float, c_ik_context*), pos, q, sequence, cycle, weight, time, ik)
};

class c_bone_merge_cache {

};

class c_base_entity : public i_client_entity {
public:
	template <typename T>
	__forceinline T& get(int offset) { return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset); }

	VFUNC(get_pred_desc_map(), 17, data_map_t*(__thiscall*)(void*))

	NETVAR(get_last_made_noise_time(), float, "CBaseEntity->m_flLastMadeNoiseTime")
	NETVAR(get_rotation(), qangle_t, "CBaseEntity->m_angRotation")
	NETVAR(get_team(), e_team_id, "CBaseEntity->m_iTeamNum")
	NETVAR(get_origin(), vec3_t, "CBaseEntity->m_vecOrigin")
	NETVAR(get_owner_entity(), c_base_handle , "CBaseEntity->m_hOwnerEntity")
	NETVAR(get_thrower_handle(), c_base_handle, "CBaseGrenade->m_hThrower");
	NETVAR(get_model_index(), int, "CBaseEntity->m_nModelIndex")
	NETVAR(is_spotted(), bool, "CBaseEntity->m_bSpotted")

	NETVAR(get_sim_time(), float, "CBaseEntity->m_flSimulationTime")
	NETVAR_OFFSET(get_old_sim_time(), float, "CBaseEntity->m_flSimulationTime", 0x4)
	NETVAR_OFFSET(get_rgfl_coordinate_frame(), matrix3x4_t, "CBaseEntity->m_CollisionGroup", -0x30)

	VFUNC(get_abs_angles(), 11, qangle_t&(__thiscall*)(void*))
	VFUNC(get_abs_origin(), 10, vec3_t&(__thiscall*)(void*))

	VFUNC(is_player(), 157, bool(__thiscall*)(void*))
	VFUNC(is_weapon(), 165, bool(__thiscall*)(void*))
	VFUNC(set_model_index(int id), 75, void(__thiscall*)(void*, int), id)

	VFUNC_SIG(set_abs_angles(const qangle_t& angles), "client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1", void(__thiscall*)(void*, const qangle_t&), angles)
	VFUNC_SIG(set_abs_origin(const vec3_t& origin), "client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8 ? ? ? ? 8B 7D", void(__thiscall*)(void*, const vec3_t&), origin)
	VFUNC_SIG(is_breakable_game(), "client.dll", "55 8B EC 51 56 8B F1 85 F6 74 68 83 BE", bool(__thiscall*)(void*))

	OFFSET(get_renderable(), i_client_renderable*, 0x4)
	OFFSET(get_networkable(), i_client_networkable*, 0x8)

	OFFSET(get_studio_hdr(), c_studio_hdr*, 0x294C)
	POFFSET(get_bone_cache(), bone_cache_t, 0x290C + 0x4)
	OFFSET(get_occlusion_mask(), bit_flag_t<uint32_t>, 0xA24)
	OFFSET(get_occlusion_frame_count(), int, 0xA30)
	OFFSET(get_unknown_occlusion_flags(), bit_flag_t<uint32_t>, 0xA2C)
	OFFSET(get_occlusion_flags(), bit_flag_t<uint32_t>, 0xA28)
	OFFSET(get_bone_array_for_write(), matrix3x4_t*, 0x26A8)
	OFFSET(get_last_setup_bones_frame_count(), int, 0xA64 + 0x4)
	OFFSET(get_predictable(), int, 0x2EA + 0x4)
	OFFSET(get_accumulated_bone_mask(), bit_flag_t<uint32_t>, 0x269C + 0x4)
	OFFSET(get_prev_bone_mask(), bit_flag_t<uint32_t>, 0x2698 + 0x4)
	OFFSET(get_most_recent_model_bone_counter(), unsigned long, 0x268C + 0x4)
	OFFSET(get_last_setup_bones_time(), float, 0x2920 + 0x4)
	OFFSET(get_ik_context(), c_ik_context*, 0x266C + 0x4)
	OFFSET(get_setup_bones_pos(), vec3_t, 0xA68 + 0x4)
	OFFSET(get_setup_bones_quaternion(), vec4_t, 0x166C + 0x4)
	OFFSET(get_take_damage(), int, 0x280)

	DATA_MAP(get_effects(), bit_flag_t<uint32_t>, "m_fEffects")
	DATA_MAP(get_eflags(), bit_flag_t<uint32_t>, "m_iEFlags")
	DATA_MAP(get_abs_velocity(), vec3_t, "m_vecAbsVelocity")
	DATA_MAP(get_abs_rotation(), qangle_t, "m_angAbsRotation")

	__forceinline void invalidate_bone_cache() {
		static const auto most_recent_model_bone_counter = **SIG("client.dll", "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81").self_offset(0xA).cast<unsigned long**>();

		get_last_setup_bones_time() = std::numeric_limits<float>::lowest();
		get_most_recent_model_bone_counter() = most_recent_model_bone_counter - 1ul;
	}

	bool is_enemy();

	__forceinline bool is_breakable() {
		if (!this
			|| !get_index())
			return false;

		if (is_breakable_game())
			return true;

		const auto client_class = get_client_class();
		if (!client_class)
			return false;

		return client_class->m_class_id == C_BASE_DOOR 
			|| client_class->m_class_id == C_BREAKABLE_SURFACE
			|| client_class->m_class_id == C_FUNC_BRUSH
			|| client_class->m_class_id == C_BASE_ENTITY && get_collideable()->get_solid() == SOLID_BSP;
	}
};

class c_inferno : public c_base_entity {
public:
	ANETVAR(get_fire_x_delta(), int, 64, "CInferno->m_fireXDelta")
	ANETVAR(get_fire_y_delta(), int, 64, "CInferno->m_fireYDelta")
	ANETVAR(get_fire_z_delta(), int, 64, "CInferno->m_fireZDelta")
};

class c_smoke_grenade_projectile : public c_base_entity {
public:
	NETVAR(did_smoke_effect(), bool, "CSmokeGrenadeProjectile->m_bDidSmokeEffect")
};

class c_base_view_model : public c_base_entity {
public:
	NETVAR(get_owner(), c_base_handle, "CBaseViewModel->m_hOwner")
	NETVAR(get_weapon(), c_base_handle, "CBaseViewModel->m_hWeapon")
	NETPROP(get_sequence_prop(), "CBaseViewModel->m_nSequence")
};

class c_base_weapon_world_model : public c_base_entity {
public:

};

class c_planted_c4 : public c_base_entity {
public:
	NETVAR(is_ticking(), bool, "CPlantedC4->m_bBombTicking")
	NETVAR(get_site(), int, "CPlantedC4->m_nBombSite")
	NETVAR(get_c4_blow(), float, "CPlantedC4->m_flC4Blow")
	NETVAR(get_timer_length(), float, "CPlantedC4->m_flTimerLength")
	NETVAR(get_defuse_length(), float, "CPlantedC4->m_flDefuseLength")
	NETVAR(get_defuse_countdown(), float, "CPlantedC4->m_flDefuseCountDown")
	NETVAR(is_defused(), bool, "CPlantedC4->m_bBombDefused")
	NETVAR(get_defuser(), c_base_handle, "CPlantedC4->m_hBombDefuser")
};

class c_base_attributable_item : public c_base_entity {
public:
	NETVAR(get_item_definition_index(), short, "CBaseAttributableItem->m_iItemDefinitionIndex")
	NETVAR(get_account_id(), int, "CBaseAttributableItem->m_iAccountID")
	NETVAR(get_item_quality(), int, "CBaseAttributableItem->m_iEntityQuality")
	NETVAR(get_item_id_high(), int, "CBaseAttributableItem->m_iItemIDHigh")
	NETVAR(get_item_id_low(), int, "CBaseAttributableItem->m_iItemIDLow")
	NETVAR(get_fallback_paint_kit(), int, "CBaseAttributableItem->m_nFallbackPaintKit")
	NETVAR(get_fallback_seed(), int, "CBaseAttributableItem->m_nFallbackSeed")
	NETVAR(get_fallback_wear(), float, "CBaseAttributableItem->m_flFallbackWear")
	NETVAR(get_fallback_stat_trak(), int, "CBaseAttributableItem->m_nFallbackStatTrak")
};

class c_base_combat_weapon;

class c_base_combat_character : public c_base_entity {
public:
	PNETVAR(get_weapons(), c_base_handle, "CBaseCombatCharacter->m_hMyWeapons")
	PNETVAR(get_wearables(), c_base_handle, "CBaseCombatCharacter->m_hMyWearables")
	NETVAR(get_active_weapon_handle(), c_base_handle, "CBaseCombatCharacter->m_hActiveWeapon")

	c_base_combat_weapon* get_active_weapon();
};

class c_base_player : public c_base_combat_character {
public:
	DATA_MAP(get_duck_amount(), float, "m_flDuckAmount")
	DATA_MAP(get_ground_entity(), c_base_handle, "m_hGroundEntity")
	DATA_MAP(get_move_type(), uint8_t, "m_MoveType")
	DATA_MAP(get_next_attack(), float, "m_flNextAttack")

	DATA_MAP(get_impulse(), int, "m_nImpulse")
	DATA_MAP(get_buttons(), bit_flag_t<uint32_t>, "m_nButtons")
	DATA_MAP(get_buttons_last(), bit_flag_t<uint32_t>, "m_afButtonLast")
	DATA_MAP(get_buttons_pressed(), bit_flag_t<uint32_t>, "m_afButtonPressed")
	DATA_MAP(get_buttons_released(), bit_flag_t<uint32_t>, "m_afButtonReleased")
	OFFSET(get_buttons_disabled(), bit_flag_t<uint32_t>, 0x3330)
	OFFSET(get_buttons_forced(), bit_flag_t<uint32_t>, 0x3334)

	DATA_MAP(get_collision_state(), int, "m_vphysicsCollisionState")

	OFFSET(get_spawn_time(), float, 0xA370)

	NETVAR(get_fall_velocity(), float, "CBasePlayer->m_flFallVelocity")
	NETVAR(get_observer_mode(), e_observer_mode, "CBasePlayer->m_iObserverMode")
	NETVAR(get_observer_target(), c_base_handle, "CBasePlayer->m_hObserverTarget")
	NETVAR(get_flags(), bit_flag_t<uint32_t>, "CBasePlayer->m_fFlags")
	NETVAR(get_velocity(), vec3_t, "CBasePlayer->m_vecVelocity[0]")
	NETVAR(get_vehicle(), c_base_handle, "CBasePlayer->m_hVehicle")
	NETVAR(get_water_level(), int, "CBasePlayer->m_nWaterLevel")
	NETVAR(get_next_think_tick(), int, "CBasePlayer->m_nNextThinkTick")
	NETVAR(get_tick_base(), int, "CBasePlayer->m_nTickBase")
	NETVAR(get_duck_speed(), float, "CBasePlayer->m_flDuckSpeed")
	NETVAR(get_view_offset(), vec3_t, "CBasePlayer->m_vecViewOffset[0]")
	NETVAR(get_health(), int, "CBasePlayer->m_iHealth")
	NETVAR(get_life_state(), e_life_state, "CBasePlayer->m_lifeState")
	NETVAR(get_view_punch_angle(), qangle_t, "CBasePlayer->m_viewPunchAngle")
	NETVAR(get_aim_punch_angle(), qangle_t, "CBasePlayer->m_aimPunchAngle")
	NETVAR(get_aim_punch_angle_vel(), vec3_t, "CBasePlayer->m_aimPunchAngleVel")
	NETVAR(get_view_model(), c_base_handle, "CBasePlayer->m_hViewModel[0]")
	NETVAR_OFFSET(get_cur_cmd(), c_user_cmd*, "CBasePlayer->m_hConstraintEntity", -0xC)
		
	OFFSET(get_last_cmd(), c_user_cmd, 0x3288)

	VFUNC(think(), 138, void(__thiscall*)(void*))
	VFUNC(pre_think(), 317, void(__thiscall*)(void*))
	VFUNC(post_think(), 318, void(__thiscall*)(void*))
	VFUNC(select_item(const char* name, int sub_type), 329, void(__thiscall*)(void*, const char*, int), name, sub_type)
	VFUNC(update_collistion_bounds(), 339, void(__thiscall*)(void*))
	VFUNC(set_local_view_angles(const qangle_t& angle), 372, void(__thiscall*)(void*, const qangle_t&), angle)

	VFUNC_SIG(unknown_think(int unk), "client.dll", "55 8B EC 56 57 8B F9 8B B7 ? ? ? ? 8B C6 C1 E8 16 24 01 74 18", void(__thiscall*)(void*, int), unk)
	VFUNC_SIG(using_standard_weapons_in_vehicle(), "client.dll", "56 57 8B F9 8B 97 ? ? ? ? 83 FA FF 74 41", bool(__thiscall*)(void*))
	VFUNC_SIG(physics_run_think(int index), "client.dll", "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87", bool(__thiscall*)(void*, int), index)
	VFUNC_SIG(post_think_v_physics(), "client.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB", bool(__thiscall*)(void*))
	VFUNC_SIG(simulate_player_simulated_entities(), "client.dll", "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 72", bool(__thiscall*)(void*))

	__forceinline bool is_alive() { return get_life_state() == LIFE_ALIVE && get_health(); }

	__forceinline vec3_t get_bone_position(int id) {
		static const auto get_bone_position_fn = SIG("client.dll", "55 8B EC 83 E4 F8 83 EC 30 8D").cast<void(__thiscall*)(void*, int, vec3_t*, vec3_t*)>();
		
		vec3_t position, rotation;

		get_bone_position_fn(this, id, &position, &rotation);

		return position;
	}

	__forceinline vec3_t get_eye_position() {
		vec3_t ret;
		memory::get_vfunc<void(__thiscall*)(void*, vec3_t&)>(this, 284)(this, ret);

		const auto view_offset = get_view_offset();

		ret.z -= view_offset.z - math::floor(view_offset.z);

		return ret;
	}
};

class c_base_animating : public c_base_player {
public:
	PPOFFSET(get_anim_layers(), anim_layers_t, 0x2980)
	NETVAR(get_pose_params(), pose_params_t, "CBaseAnimating->m_flPoseParameter")
	NETPROP(get_client_side_animation_prop(), "CBaseAnimating->m_bClientSideAnimation")
	NETVAR(get_client_side_animation(), bool, "CBaseAnimating->m_bClientSideAnimation")
	NETVAR(get_sequence(), int, "CBaseAnimating->m_nSequence")
		
	VFUNC(set_sequence(int sequence), 218, void(__thiscall*)(void*, int), sequence)
	VFUNC(studio_frame_advance(), 219, void(__thiscall*)(void*))
	VFUNC(get_layer_sequence_cycle_rate(anim_layer_t* layer, int sequence), 222, float(__thiscall*)(void*, anim_layer_t*, int), layer, sequence)

	POFFSET(get_bone_accessor(), bone_accessor_t, 0x26A4)
	OFFSET(get_bone_merge_cache(), c_bone_merge_cache*, 0x290C)

	__forceinline int select_weighted_sequence_from_modifiers(int activity, void* modifiers, int count) {
		const auto hdr = get_studio_hdr();
		if (!hdr)
			return -1;

		if (!hdr->m_activity_to_sequence) {
			hdr->m_activity_to_sequence = hdr->find_mapping();
		}

		return hdr->m_activity_to_sequence->select_weighted_sequence_from_modifiers(hdr, activity, modifiers, count);
	}

	__forceinline void set_pose_parameter(int param, float value) {
		static const auto studio_set_pose_parameter_fn = SIG("client.dll", "55 8B EC 83 E4 F8 83 EC 08 F3 0F 11 54 24 ? 85 D2").get();

		auto result = 0.f;
		auto hdr = get_studio_hdr();

		__asm {
			pushad
			movss xmm2, [value]
			lea eax, [result]
			push eax
			mov edx, param
			mov ecx, hdr
			call studio_set_pose_parameter_fn
			pop eax
			popad
		}

		get_pose_params()[param] = result;
	}

	int get_sequence_activity(int sequence);
};

class c_cs_player : public c_base_animating {
public:
	NETVAR(get_velocity_modifier(), float, "CCSPlayer->m_flVelocityModifier")
	NETVAR(has_defuser(), bool, "CCSPlayer->m_bHasDefuser")
        NETVAR(is_strafing(), bool, "CCSPlayer->m_bStrafing)
	NETVAR(has_helmet(), bool, "CCSPlayer->m_bHasHelmet")
	NETVAR(get_armor_value(), int, "CCSPlayer->m_ArmorValue")
	NETVAR(has_heavy_armor(), bool, "CCSPlayer->m_bHasHeavyArmor")
	NETVAR(get_eye_angles(), qangle_t, "CCSPlayer->m_angEyeAngles")
	NETVAR(is_scoped(), bool, "CCSPlayer->m_bIsScoped")
	NETVAR(is_immune(), bool, "CCSPlayer->m_bGunGameImmunity")
	NETVAR(is_ghost(), bool, "CCSPlayer->m_bIsPlayerGhost")
	NETVAR(get_health_boost_time(), float, "CCSPlayer->m_flHealthShotBoostExpirationTime")
	NETVAR(get_lby(), float, "CCSPlayer->m_flLowerBodyYawTarget")
	NETVAR_OFFSET(get_flash_alpha(), float, "CCSPlayer->m_flFlashMaxAlpha", -0x8)

	OFFSET(get_anim_state(), c_anim_state*, 0x3914)

	VFUNC(standard_blending_rules(c_studio_hdr* hdr, vec3_t* vec, vec4_t* q, float time, int mask), 205, void(__thiscall*)(void*, c_studio_hdr*, vec3_t*, vec4_t*, float, int), hdr, vec, q, time, mask)
	VFUNC(build_transformations(c_studio_hdr* hdr, vec3_t* vec, vec4_t* q, matrix3x4_t& transform, int mask, uint8_t* computed), 189, void(__thiscall*)(void*, c_studio_hdr*, vec3_t*, vec4_t*, matrix3x4_t const&, int, uint8_t*), hdr, vec, q, transform, mask, computed)
	VFUNC(update_ik_locks(float time), 191, void(__thiscall*)(void*, float), time)
	VFUNC(calculate_ik_locks(float time), 192, void(__thiscall*)(void*, float), time)
	VFUNC(update_client_side_animation(), 223, void(__thiscall*)(void*))

	VFUNC_SIG(invalidate_physics_recursive(int flags), "client.dll", "55 8B EC 83 E4 F8 83 EC 0C 53 8B 5D 08 8B C3 56", void(__thiscall*)(void*, int), flags)

	player_info_t get_info();

	__forceinline static uintptr_t* get_vtable() {
		static const auto vtable = SIG("client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C").self_offset(0x47).cast<uintptr_t*>();

		return vtable;
	}
};

class c_base_combat_weapon : public c_base_attributable_item {
public:
	VFUNC(get_inaccuracy(), 482, float(__thiscall*)(void*))
	VFUNC(get_spread(), 452, float(__thiscall*)(void*))
	VFUNC(update_accuracy(), 483, void(__thiscall*)(void*))

	NETVAR(get_pin_pulled(), bool, "CBaseCSGrenade->m_bPinPulled")
	NETVAR(get_throw_time(), float, "CBaseCSGrenade->m_fThrowTime")

	NETVAR(get_post_pone_fire_ready_time(), float, "CWeaponCSBase->m_flPostponeFireReadyTime")
	NETVAR(get_accuracy_penalty(), float, "CWeaponCSBase->m_fAccuracyPenalty")
	NETVAR(get_recoil_index(), float, "CWeaponCSBase->m_flRecoilIndex")
	NETVAR(get_last_shot_time(), float, "CWeaponCSBase->m_fLastShotTime")
	NETVAR(get_throw_strength(), float, "CWeaponCSBase->m_flThrowStrength")
	NETVAR(get_ammo(), int, "CBaseCombatWeapon->m_iClip1")
	NETVAR(get_zoom_level(), int, "CWeaponCSBaseGun->m_zoomLevel")
	NETVAR(get_next_primary_attack(), float, "CBaseCombatWeapon->m_flNextPrimaryAttack")
	NETVAR(get_next_secondary_attack(), float, "CBaseCombatWeapon->m_flNextSecondaryAttack")
	NETVAR(get_world_model(), c_base_handle, "CBaseCombatWeapon->m_hWeaponWorldModel")

	std::wstring get_name();

	c_cs_weapon_data* get_cs_weapon_data();
};

class c_local_player {
	friend bool operator==(const c_local_player& lhs, void* rhs) { return *lhs.m_loc == rhs; }
public:
	c_local_player() = default;

	operator bool() const { return *m_loc != nullptr; }
	operator c_cs_player*() const { return *m_loc; }

	c_cs_player* operator->() const { return *m_loc; }
private:
	c_cs_player** m_loc = nullptr;
};

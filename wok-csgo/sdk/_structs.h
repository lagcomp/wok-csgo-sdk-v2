#pragma once

struct jiggle_data_t {
	jiggle_data_t(int bone, float cur_time, const vec3_t& base_pos, const vec3_t& tip_pos) {
		m_bone = bone;

		m_last_update = cur_time;

		m_base_pos = m_base_last_pos = base_pos;
		m_base_velocity = m_base_acceleration = vec3_t();

		m_tip_pos = tip_pos;
		m_tip_velocity = m_tip_acceleration = vec3_t();
	}

	int		m_bone;
	int		m_id;
	float	m_last_update;
	vec3_t	m_base_pos;
	vec3_t	m_base_last_pos;
	vec3_t	m_base_velocity;
	vec3_t	m_base_acceleration;
	vec3_t	m_tip_pos;
	vec3_t	m_tip_velocity;
	vec3_t	m_tip_acceleration;
};

struct player_info_t {
	int64_t			pad0;

	union {
		int64_t		m_xuid;
		struct {
			int		m_xuid_low;
			int		m_xuid_high;
		};
	};

	char			m_name[128];
	int				m_user_id;
	char			m_guid[33];
	uint32_t		m_friends_id;
	char			m_friends_name[128];
	bool			m_fake_player;
	bool			m_is_hltv;
	uint32_t		m_custom_files[4];
	unsigned char	m_files_downloaded;
};

struct bone_cache_t {
	matrix3x4_t*	m_cached_bones;
	char			pad0[8];
	uint32_t		m_cached_bones_count;
};

class c_base_entity;
class c_base_animating;

struct bone_accessor_t {
	c_base_animating*	m_animating;
	matrix3x4a_t*		m_bones;
	int					m_readable_bones;
	int					m_writable_bones;
};

struct anim_layer_t {
	float			m_anim_time;
	float			m_fade_out_time;
	int				m_flags;
	int				m_activity;
	int				m_priority;
	int				m_order;
	int				m_sequence;
	float			m_prev_cycle;
	float			m_weight;
	float			m_weight_delta_rate;
	float			m_playback_rate;
	float			m_cycle;
	c_base_entity*	m_owner;
	int				m_invalidate_physics_bits;
};

class c_base_combat_weapon;

class c_anim_state {
public:
	VFUNC_SIG(reset(), "client.dll", "56 6A 01 68 ? ? ? ? 8B F1", void(__thiscall*)(void*))
	VFUNC_SIG(create(c_base_entity* entity), "client.dll", "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46", void(__thiscall*)(void*, c_base_entity*), entity)
	VFUNC_SIG(get_weapon_prefix(), "client.dll", "53 56 57 8B F9 33 F6 8B 4F 60 8B 01 FF 90", const char*(__thiscall*)(void*))
	
	__forceinline float get_body_yaw_modifier() const {
		const auto walk_speed = math::clamp(m_speed_as_portion_of_walk_speed, 0.f, 1.f);

		const auto run_speed = ((m_walk_to_run_transition * -0.30000001f) - 0.19999999f) * walk_speed;
		const auto modifier = run_speed + 1.f;

		if (m_duck_amount > 0.f) {
			const auto crouch_walk_speed = math::clamp(m_speed_as_portion_of_crouch_speed, 0.f, 1.f);

			return modifier + (m_duck_amount * crouch_walk_speed) * (0.5f - modifier);
		}

		return modifier;
	}

	__forceinline void update(qangle_t angle) {
		static const auto update_fn = SIG("client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24").get();
		if (!update_fn)
			return;

		__asm {
			push 0

			mov ecx, this

			movss xmm1, dword ptr [angle + 4]
			movss xmm2, dword ptr [angle]

			call update_fn
		}
	}

	char					pad0[4];
	bool					m_reset;
	char					pad1[3];
	int						m_tick_count;
	char					pad2[83];
	c_base_entity*			m_base_entity;
	c_base_combat_weapon*	m_active_weapon;
	c_base_combat_weapon*	m_last_active_weapon;
	float					m_last_cur_time;
	int						m_last_frame_count;
	float					m_delta_time;
	float					m_eye_yaw;
	float					m_eye_pitch;
	float					m_goal_feet_yaw;
	float					m_last_feet_yaw;
	float					m_cur_torso_yaw;
	float					m_velocity_lean;
	float					m_lean_amount;
	char					pad3[4];
	float					m_feet_cycle;
	float					m_feet_weight;
	char					pad4[4];
	float					m_duck_amount;
	float					m_landing_duck_additive;
	char					pad5[4];
	vec3_t					m_origin;
	vec3_t					m_last_origin;
	vec3_t					m_velocity;
	vec3_t					m_velocity_normalized;
	vec3_t					m_velocity_normalized_non_zero;
	float					m_speed_2d;
	float					m_up_velocity;
	float					m_speed_as_portion_of_run_speed;
	float					m_speed_as_portion_of_walk_speed;
	float					m_speed_as_portion_of_crouch_speed;
	float					m_time_since_started_moving;
	float					m_time_since_stopped_moving;
	bool					m_on_ground;
	bool					m_in_hit_ground_animation;
	char					pad6[6];
	float					m_time_since_in_air;
	float					m_last_origin_z;
	float					m_head_height_or_offset_from_hitting_ground_animation;
	float					m_walk_to_run_transition;
	char					pad7[4];
	float					m_magic_fraction;
	bool					m_on_ladder;
	char					pad8[47];
	float					m_last_velocity_test_time;
	vec3_t					m_last_velocity;
	vec3_t					m_dst_acceleration;
	vec3_t					m_acceleration;
	float					m_acceleration_weight;
	char					pad9[428];
	float					m_min_body_yaw;
	float					m_max_body_yaw;
	float					m_min_body_pitch;
	float					m_max_body_pitch;
	int						m_anim_set_version;
};

class c_cs_weapon_data {
public:
	char	pad0[4];
	char*	m_console_name;
	char	pad1[12];
	int		m_max_clip1;
	int		m_max_clip2;
	int		m_default_clip1;
	int		m_default_clip2;
	int		m_primary_reserve_ammo_max;
	int		m_secondary_reserve_ammo_max;
	char*	m_world_model;
	char*	m_view_model;
	char*	m_dropped_model;
	char	pad2[80];
	char*	m_hud_name;
	char*	m_weapon_name;
	char	pad3[2];
	bool	m_is_melee_weapon;
	char	pad4[9];
	float	m_weapon_weight;
	char	pad5[40];
	int		m_weapon_type;
	char	pad6[4];
	int		m_weapon_price;
	int		m_kill_award;
	char	pad7[4];
	float	m_cycle_time;
	float	m_cycle_time_alt;
	char	pad8[8];
	bool	m_full_auto;
	char	pad9[3];
	int		m_damage;
	float	m_armor_ratio;
	int		m_bullets;
	float	m_penetration;
	char	pad10[8];
	float	m_range;
	float	m_range_modifier;
	float	m_throw_velocity;
	char	pad11[12];
	bool	m_has_silencer;
	char	pad12[11];
	char*	m_bullet_type;
	float	m_max_speed;
	float	m_max_speed_alt;
	char	pad13[4];
	float	m_spread;
	float	m_spread_alt;
	float	m_inaccuracy_crouch;
	float	m_inaccuracy_crouch_alt;
	float	m_inaccuracy_stand;
	float	m_inaccuracy_stand_alt;
	float	m_inaccuracy_jump_initial;
	float	m_inaccuracy_jump_apex;
	float	m_inaccuracy_jump;
	float	m_inaccuracy_jump_alt;
	float	m_inaccuracy_land;
	float	m_inaccuracy_land_alt;
	float	m_inaccuracy_ladder;
	float	m_inaccuracy_ladder_alt;
	float	m_inaccuracy_fire;
	float	m_inaccuracy_fire_alt;
	float	m_inaccuracy_move;
	float	m_inaccuracy_move_alt;
	float	m_inaccuracy_reload;
	int		m_recoil_seed;
	float	m_recoil_angle;
	float	m_recoil_angle_alt;
	float	m_recoil_angle_variance;
	float	m_recoil_angle_variance_alt;
	float	m_recoil_magnitude;
	float	m_recoil_magnitude_alt;
	float	m_recoil_magnitude_variance;
	float	m_recoil_magnitude_variance_alt;
};

enum e_cs_weapon_type {
	WEAPON_TYPE_KNIFE,
	WEAPON_TYPE_PISTOL,
	WEAPON_TYPE_SMG,
	WEAPON_TYPE_RIFLE,
	WEAPON_TYPE_SHOTGUN,
	WEAPON_TYPE_SNIPER,
	WEAPON_TYPE_MACHINE_GUN,
	WEAPON_TYPE_C4,
	WEAPON_TYPE_PLACE_HOLDER,
	WEAPON_TYPE_GRENADE,
	WEAPON_TYPE_UNKNOWN,
	WEAPON_TYPE_MEDISHOT,
	WEAPON_TYPE_FISTS,
	WEAPON_TYPE_BREACH_CHARGE,
	WEAPON_TYPE_BUMP_MINE,
	WEAPON_TYPE_TABLET,
	WEAPON_TYPE_MELEE
};

enum e_material_flags {
	MATERIAL_FLAG_DEBUG = (1 << 0),
	MATERIAL_FLAG_NO_DEBUG_OVERRIDE = (1 << 1),
	MATERIAL_FLAG_NO_DRAW = (1 << 2),
	MATERIAL_FLAG_USE_IN_FILLRATE_MODE = (1 << 3),
	MATERIAL_FLAG_VERTEXCOLOR = (1 << 4),
	MATERIAL_FLAG_VERTEXALPHA = (1 << 5),
	MATERIAL_FLAG_SELFILLUM = (1 << 6),
	MATERIAL_FLAG_ADDITIVE = (1 << 7),
	MATERIAL_FLAG_ALPHATEST = (1 << 8),
	MATERIAL_FLAG_ZNEARER = (1 << 10),
	MATERIAL_FLAG_MODEL = (1 << 11),
	MATERIAL_FLAG_FLAT = (1 << 12),
	MATERIAL_FLAG_NOCULL = (1 << 13),
	MATERIAL_FLAG_NOFOG = (1 << 14),
	MATERIAL_FLAG_IGNOREZ = (1 << 15),
	MATERIAL_FLAG_DECAL = (1 << 16),
	MATERIAL_FLAG_ENVMAPSPHERE = (1 << 17),
	MATERIAL_FLAG_ENVMAPCAMERASPACE = (1 << 19),
	MATERIAL_FLAG_BASEALPHAENVMAPMASK = (1 << 20),
	MATERIAL_FLAG_TRANSLUCENT = (1 << 21),
	MATERIAL_FLAG_NORMALMAPALPHAENVMAPMASK = (1 << 22),
	MATERIAL_FLAG_NEEDS_SOFTWARE_SKINNING = (1 << 23),
	MATERIAL_FLAG_OPAQUETEXTURE = (1 << 24),
	MATERIAL_FLAG_ENVMAPMODE = (1 << 25),
	MATERIAL_FLAG_SUPPRESS_DECALS = (1 << 26),
	MATERIAL_FLAG_HALFLAMBERT = (1 << 27),
	MATERIAL_FLAG_WIREFRAME = (1 << 28),
	MATERIAL_FLAG_ALLOWALPHATOCOVERAGE = (1 << 29),
	MATERIAL_FLAG_ALPHA_MODIFIED_BY_PROXY = (1 << 30),
	MATERIAL_FLAG_VERTEXFOG = (1 << 31)
};

enum e_invalidate_physics_bits {
	POSITION_CHANGED 	= (1 << 0),
	ANGLES_CHANGED 		= (1 << 1),
	VELOCITY_CHANGED 	= (1 << 2),
	ANIMATION_CHANGED 	= (1 << 3),
	BOUNDS_CHANGED 		= (1 << 4),
	SEQUENCE_CHANGED 	= (1 << 5)
};

enum e_cvar_flags {
	FCVAR_NONE,
	FCVAR_UNREGISTERED				= (1 << 0),
	FCVAR_DEVELOPMENT_ONLY			= (1 << 1),
	FCVAR_GAME_DLL					= (1 << 2),
	FCVAR_CLIENT_DLL				= (1 << 3),
	FCVAR_HIDDEN					= (1 << 4),
	FCVAR_PROTECTED					= (1 << 5),
	FCVAR_SINGLEPLAYER_ONLY			= (1 << 6),
	FCVAR_ARCHIVE					= (1 << 7),
	FCVAR_NOTIFY					= (1 << 8),
	FCVAR_USER_INFO					= (1 << 9),
	FCVAR_PRINTABLE_ONLY			= (1 << 10),
	FCVAR_GAMEDLL_FOR_CLIENTS		= (1 << 10),
	FCVAR_UNLOGGED					= (1 << 11),
	FCVAR_NEVER_AS_STRING			= (1 << 12),
	FCVAR_REPLICATED				= (1 << 13),
	FCVAR_CHEAT						= (1 << 14),
	FCVAR_SS						= (1 << 15),
	FCVAR_DEMO						= (1 << 16),
	FCVAR_DONT_RECORD				= (1 << 17),
	FCVAR_SS_ADDED					= (1 << 18),
	FCVAR_RELEASE					= (1 << 19),
	FCVAR_RELOAD_MATERIALS			= (1 << 20),
	FCVAR_RELOAD_TEXTURES			= (1 << 21),
	FCVAR_NOT_CONNECTED				= (1 << 22),
	FCVAR_MATERIAL_SYSTEM_THREAD	= (1 << 23),
	FCVAR_ARCHIVE_GAME_CONSOLE		= (1 << 24),
	FCVAR_ACCESSIBLE_FROM_THREADS	= (1 << 25),
	FCVAR_SERVER_CAN_EXECUTE		= (1 << 28),
	FCVAR_SERVER_CANNOT_QUERY		= (1 << 29),
	FCVAR_CLIENTCMD_CAN_EXECUTE		= (1 << 30),
	FCVAR_MATERIAL_THREAD_MASK		= (FCVAR_RELOAD_MATERIALS | FCVAR_RELOAD_TEXTURES | FCVAR_MATERIAL_SYSTEM_THREAD)
};

enum e_class_id {
	C_AI_BASE_NPC,
	C_WEAPON_AK47,
	C_BASE_ANIMATING,
	C_BASE_ANIMATING_OVERLAY,
	C_BASE_ATTRIBUTABLE_ITEM,
	C_BASE_BUTTON,
	C_BASE_COMBAT_CHARACTER,
	C_BASE_COMBAT_WEAPON,
	C_BASE_CS_GRENADE,
	C_BASE_CS_GRENADE_PROJECTILE,
	C_BASE_DOOR,
	C_BASE_ENTITY,
	C_BASE_FLEX,
	C_BASE_GRENADE,
	C_BASE_PARTICLE_ENTITY,
	C_BASE_PLAYER,
	C_BASE_PROP_DOOR,
	C_BASE_TEAM_OBJECTIVE_RESOURCE,
	C_BASE_TEMP_ENTITY,
	C_BASE_TOGGLE,
	C_BASE_TRIGGER,
	C_BASE_VIEWMODEL,
	C_BASE_VPHYSICS_TRIGGER,
	C_BASE_WEAPON_WORLD_MODEL,
	C_BEAM,
	C_BEAM_SPOTLIGHT,
	C_BONE_FOLLOWER,
	C_BR_C4_TARGET,
	C_BREACH_CHARGE,
	C_BREACH_CHARGE_PROJECTILE,
	C_BREAKABLE_PROP,
	C_BREAKABLE_SURFACE,
	C_BUMP_MINE,
	C_BUMP_MINE_PROJECTILE,
	C_C4,
	C_CASCADE_LIGHT,
	C_CHICKEN,
	C_COLOR_CORRECTION,
	C_COLOR_CORRECTION_VOLUME,
	C_CS_GAMERULES_PROXY,
	C_CS_PLAYER,
	C_CS_PLAYER_RESOURCE,
	C_CS_RAGDOLL,
	C_CS_TEAM,
	C_DANGERZONE,
	C_DANGERZONE_CONTROLLER,
	C_DEAGLE,
	C_DECOY_GRENADE,
	C_DECOY_PROJECTILE,
	C_DRONE,
	C_DRONE_GUN,
	C_DYNAMIC_LIGHT,
	C_DYNAMIC_PROP,
	C_ECON_ENTITY,
	C_ECON_WEARABLE,
	C_EMBERS,
	C_ENTITY_DISSOLVE,
	C_ENTITY_FLAME,
	C_ENTITY_FREEZING,
	C_ENTITY_PARTICLE_TRAIL,
	C_ENV_AMBIENT_LIGHT,
	C_ENV_DETAIL_CONTROLLER,
	C_ENV_DOF_CONTROLLER,
	C_ENV_GAS_CANISTER,
	C_ENV_PARTICLE_SCRIPT,
	C_ENV_PROJECTED_TEXTURE,
	C_ENV_QUADRATIC_BEAM,
	C_ENV_SCREEN_EFFECT,
	C_ENV_SCREEN_OVERLAY,
	C_ENV_TONEMAP_CONTROLLER,
	C_ENV_WIND,
	C_FE_PLAYER_DECAL,
	C_FIRECRACKER_BLAST,
	C_FIRE_SMOKE,
	C_FIRE_TRAIL,
	C_FISH,
	C_FISTS,
	C_FLASHBANG,
	C_FOG_CONTROLLER,
	C_FOOTSTEP_CONTROL,
	C_FUNC_DUST,
	C_FUNC_LOD,
	C_FUNC_AREA_PORTAL_WINDOW,
	C_FUNC_BRUSH,
	C_FUNC_CONVEYOR,
	C_FUNC_LADDER,
	C_FUNC_MONITOR,
	C_FUNC_MOVE_LINEAR,
	C_FUNC_OCCLUDER,
	C_FUNC_REFLECTIVE_GLASS,
	C_FUNC_ROTATING,
	C_FUNC_SMOKE_VOLUME,
	C_FUNC_TRACK_TRAIN,
	C_GAMERULES_PROXY,
	C_GRASS_BURN,
	C_HANDLE_TEST,
	C_HE_GRENADE,
	C_HOSTAGE,
	C_HOSTAGE_CARRIABLE_PROP,
	C_INCENDIARY_GRENADE,
	C_INFERNO,
	C_INFO_LADDER_DISMOUNT,
	C_INFO_MAP_REGION,
	C_INFO_OVERLAY_ACCESSOR,
	C_ITEM_HEALTHSHOT,
	C_ITEM_CASH,
	C_ITEM_DOGTAGS,
	C_KNIFE,
	C_KNIFEGG,
	C_LIGHT_GLOW,
	C_MATERIAL_MODIFY_CONTROL,
	C_MELEE,
	C_MOLOTOV_GRENADE,
	C_MOLOTOV_PROJECTILE,
	C_MOVIE_DISPLAY,
	C_PARADROP_CHOPPER,
	C_PARTICLE_FIRE,
	C_PARTICLE_PERFORMANCE_MONITOR,
	C_PARTICLE_SYSTEM,
	C_PHYS_BOX,
	C_PHYS_BOX_MULTIPLAYER,
	C_PHYSICS_PROP,
	C_PHYSICS_PROP_MULTIPLAYER,
	C_PHYS_MAGNET,
	C_PHYS_PROP_AMMO_BOX,
	C_PHYS_PROP_LOOT_CRATE,
	C_PHYS_PROP_RADAR_JAMMER,
	C_PHYS_PROP_WEAPON_UPGRADE,
	C_PLANTED_C4,
	C_PLASMA,
	C_PLAYER_PING,
	C_PLAYER_RESOURCE,
	C_POINT_CAMERA,
	C_POINT_COMMENT_ARYNODE,
	C_POINT_WORLD_TEXT,
	C_POSE_CONTROLLER,
	C_POST_PROCESS_CONTROLLER,
	C_PRECIPITATION,
	C_PRECIPITATION_BLOCKER,
	C_PREDICTED_VIEWMODEL,
	C_PROP_HALLUCINATION,
	C_PROP_COUNTER,
	C_PROP_DOOR_ROTATING,
	C_PROP_JEEP,
	C_PROP_VEHICLE_DRIVEABLE,
	C_RAGDOLL_MANAGER,
	C_RAGDOLL_PROP,
	C_RAGDOLL_PROP_ATTACHED,
	C_ROPE_KEYFRAME,
	C_WEAPON_SCAR17,
	C_SCENE_ENTITY,
	C_SENSOR_GRENADE,
	C_SENSOR_GRENADE_PROJECTILE,
	C_SHADOW_CONTROL,
	C_SLIDE_SHOW_DISPLAY,
	C_SMOKE_GRENADE,
	C_SMOKE_GRENADE_PROJECTILE,
	C_SMOKE_STACK,
	C_SNOWBALL,
	C_SNOWBALL_PILE,
	C_SNOWBALL_PROJECTILE,
	C_SPATIAL_ENTITY,
	C_SPOTLIGHT_END,
	C_SPRITE,
	C_SPRITE_ORIENTED,
	C_SPRITE_TRAIL,
	C_STATUE_PROP,
	C_STEAMJET,
	C_SUN,
	C_SUNLIGHT_SHADOW_CONTROL,
	C_SURVIVAL_SPAWN_CHOPPER,
	C_TABLET,
	C_TEAM,
	C_TEAM_PLAYROUND_BASED_RULES_PROXY,
	C_TEAR_MORRICOCHET,
	C_TE_BASE_BEAM,
	C_TE_BEAM_ENT_POINT,
	C_TE_BEAM_ENTS,
	C_TE_BEAM_FOLLOW,
	C_TE_BEAM_LASER,
	C_TE_BEAM_POINTS,
	C_TE_BEAM_RING,
	C_TE_BEAM_RING_POINT,
	C_TE_BEAM_SPLINE,
	C_TE_BLOOD_SPRITE,
	C_TE_BLOOD_STREAM,
	C_TE_BREAK_MODEL,
	C_TE_BSP_DECAL,
	C_TE_BUBBLES,
	C_TE_BUBBLE_TRAIL,
	C_TE_CLIENT_PROJECTILE,
	C_TE_DECAL,
	C_TE_DUST,
	C_TE_DYNAMIC_LIGHT,
	C_TE_EFFECT_DISPATCH,
	C_TE_ENERGY_SPLASH,
	C_TE_EXPLOSION,
	C_TE_FIRE_BULLETS,
	C_TE_FIZZ,
	C_TE_FOOTPRINT_DECAL,
	C_TE_FOUNDRY_HELPERS,
	C_TE_GAUSS_EXPLOSION,
	C_TE_GLOW_SPRITE,
	C_TE_IMPACT,
	C_TE_KILL_PLAYER_ATTACHMENTS,
	C_TE_LARGE_FUNNEL,
	C_TE_METAL_SPARKS,
	C_TE_MUZZLE_FLASH,
	C_TE_PARTICLE_SYSTEM,
	C_TE_PHYSICS_PROP,
	C_TE_PLANT_BOMB,
	C_TE_PLAYER_ANIM_EVENT,
	C_TE_PLAYER_DECAL,
	C_TE_PROJECTED_DECAL,
	C_TE_RADIOICON,
	C_TE_SHATTER_SURFACE,
	C_TE_SHOW_LINE,
	C_TE_SLA,
	C_TE_SMOKE,
	C_TE_SPARKS,
	C_TE_SPRITE,
	C_TE_SPRITE_SPRAY,
	C_TEST_PROXY_TOGGLE_NETWORKABLE,
	C_TEST_TRACE_LINE,
	C_TE_WORLD_DECAL,
	C_TRIGGER_PLAYER_MOVEMENT,
	C_TRIGGER_SOUND_OPERATOR,
	C_VGUI_SCREEN,
	C_VOTE_CONTROLLER,
	C_WATER_BULLET,
	C_WATER_LOD_CONTROL,
	C_WEAPON_AUG,
	C_WEAPON_AWP,
	C_WEAPON_BASE_ITEM,
	C_WEAPON_PPBIZON,
	C_WEAPON_CS_BASE,
	C_WEAPON_CS_BASE_GUN,
	C_WEAPON_CYCLER,
	C_WEAPON_DUAL_BERETTAS,
	C_WEAPON_FAMAS,
	C_WEAPON_FIVESEVEN,
	C_WEAPON_G3SG1,
	C_WEAPON_GALIL,
	C_WEAPON_GALIL_AR,
	C_WEAPON_GLOCK18,
	C_WEAPON_P2000,
	C_WEAPON_M249,
	C_WEAPON_M3,
	C_WEAPON_M4A4,
	C_WEAPON_MAC10,
	C_WEAPON_MAG7,
	C_WEAPON_MP5SD,
	C_WEAPON_MP7,
	C_WEAPON_MP9,
	C_WEAPON_NEGEV,
	C_WEAPON_NOVA,
	C_WEAPON_P228,
	C_WEAPON_P250,
	C_WEAPON_P90,
	C_WEAPON_SAWEDOFF,
	C_WEAPON_SCAR20,
	C_WEAPON_SCOUT,
	C_WEAPON_SG550,
	C_WEAPON_SG552,
	C_WEAPON_SG553,
	C_WEAPON_BALLISTIC_SHIELD,
	C_WEAPON_SSG08,
	C_WEAPON_ZEUSX27,
	C_WEAPON_TEC9,
	C_WEAPON_TMP,
	C_WEAPON_UMP45,
	C_WEAPON_USP,
	C_WEAPON_XM1014,
	C_WORLD,
	C_WORLD_VGUI_TEXT,
	DUST_TRAIL,
	MOVIE_EXPLOSION,
	PARTICLE_SMOKE_GRENADE,
	ROCKET_TRAIL,
	SMOKE_TRAIL,
	SPORE_EXPLOSION,
	SPORE_TRAIL
};

enum e_item_definition_index : short {
	WEAPON_NONE = 0,
	WEAPON_DESERT_EAGLE,
	WEAPON_DUAL_BERETTAS,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK18,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALIL_AR = 13,
	WEAPON_M249,
	WEAPON_M4A4 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_REPULSOR_DEVICE,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_PPBIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_ZEUS_X27,
	WEAPON_P2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_BALLISTIC_SHIELD,
	WEAPON_SCAR20,
	WEAPON_SG_553,
	WEAPON_SSG_08,
	WEAPON_KNIFE_GOLD,
	WEAPON_KNIFE_DEFAULT_CT,
	WEAPON_GRENADE_FLASHBANG,
	WEAPON_GRENADE_HE,
	WEAPON_GRENADE_SMOKE,
	WEAPON_GRENADE_MOLOTOV,
	WEAPON_GRENADE_DECOY,
	WEAPON_GRENADE_INCENDIARY,
	WEAPON_C4,
	ITEM_KEVLAR,
	ITEM_KEVLAR_HELMET,
	ITEM_HEAVY_ASSAULT_SUIT,
	ITEM_DEFUSE_KIT = 55,
	ITEM_RESCUE_KIT,
	WEAPON_MEDISHOT,
	WEAPON_KNIFE_DEFAULT_T = 59,
	WEAPON_M4A1S,
	WEAPON_USPS,
	WEAPON_CZ75A = 63,
	WEAPON_R8_REVOLVER,
	WEAPON_GRENADE_TA = 68,
	WEAPON_BARE_HANDS,
	WEAPON_BREACH_CHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_WRENCH = 78,
	WEAPON_KNIFE_SPECTRAL_SHIV = 80,
	WEAPON_GRENADE_FIRE_BOMB,
	WEAPON_GRENADE_DIVERSION_DEVICE,
	WEAPON_GRENADE_FRAG,
	WEAPON_SNOWBALL,
	WEAPON_BUMP_MINE,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_CLASSIC = 503,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_HUNTSMAN,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_SHADOW_DAGGERS,
	WEAPON_KNIFE_PARACORD,
	WEAPON_KNIFE_SURVIVAL,
	WEAPON_KNIFE_URSUS,
	WEAPON_KNIFE_NAVAJA,
	WEAPON_KNIFE_NOMAD,
	WEAPON_KNIFE_STILETTO,
	WEAPON_KNIFE_TALON,
	WEAPON_KNIFE_SKELETON = 525,
	GLOVE_BLOODHOUND = 5027,
	GLOVE_DEFAULT_T,
	GLOVE_DEFAULT_CT,
	GLOVE_SPORT,
	GLOVE_DRIVER,
	GLOVE_HAND_WRAPS,
	GLOVE_MOTO,
	GLOVE_SPECIALIST,
	GLOVE_HYDRA,
	AGENT_CT_DEFAULT,
	AGENT_T_DEFAULT,
	AGENT_T_ANARCHIST,
	AGENT_T_ANARCHIST_A,
	AGENT_T_ANARCHIST_B,
	AGENT_T_ANARCHIST_C,
	AGENT_T_ANARCHIST_D,
	AGENT_T_PIRATE,
	AGENT_T_PIRATE_A,
	AGENT_T_PIRATE_B,
	AGENT_T_PIRATE_C,
	AGENT_T_PIRATE_D,
	AGENT_T_PROFESSIONAL,
	AGENT_T_PROFESSIONAL_A,
	AGENT_T_PROFESSIONAL_B,
	AGENT_T_PROFESSIONAL_C,
	AGENT_T_PROFESSIONAL_D,
	AGENT_T_SEPARATIST,
	AGENT_T_SEPARATIST_A,
	AGENT_T_SEPARATIST_B,
	AGENT_T_SEPARATIST_C,
	AGENT_T_SEPARATIST_D,
	AGENT_CT_GIGN,
	AGENT_CT_GIGN_A,
	AGENT_CT_GIGN_B,
	AGENT_CT_GIGN_C,
	AGENT_CT_GIGN_D,
	AGENT_CT_GSG9,
	AGENT_CT_GSG9_A,
	AGENT_CT_GSG9_B,
	AGENT_CT_GSG9_C,
	AGENT_CT_GSG9_D,
	AGENT_CT_IDF,
	AGENT_CT_IDF_B,
	AGENT_CT_IDF_C,
	AGENT_CT_IDF_D,
	AGENT_CT_IDF_E,
	AGENT_CT_IDF_F,
	AGENT_CT_SWAT,
	AGENT_CT_SWAT_A,
	AGENT_CT_SWAT_B,
	AGENT_CT_SWAT_C,
	AGENT_CT_SWAT_D,
	AGENT_CT_SAS_A,
	AGENT_CT_SAS_B,
	AGENT_CT_SAS_C,
	AGENT_CT_SAS_D,
	AGENT_CT_ST6,
	AGENT_CT_ST6_A,
	AGENT_CT_ST6_B,
	AGENT_CT_ST6_C,
	AGENT_CT_ST6_D,
	AGENT_T_BALKAN,
	AGENT_T_BALKAN_A,
	AGENT_T_BALKAN_B,
	AGENT_T_BALKAN_C,
	AGENT_T_BALKAN_D,
	AGENT_T_JUMPSUIT_A,
	AGENT_T_JUMPSUIT_B,
	AGENT_T_JUMPSUIT_C,
	AGENT_T_HEAVY,
	AGENT_CT_HEAVY,
	AGENT_T_LEET_A = 5100,
	AGENT_T_LEET_B,
	AGENT_T_LEET_C,
	AGENT_T_LEET_D,
	AGENT_T_LEET_E,
	AGENT_T_ELITE_CREW_GROUND_REBEL,
	AGENT_T_ELITE_CREW_OSIRIS,
	AGENT_T_ELITE_CREW_PROF_SHAHMAT,
	AGENT_T_ELITE_CREW_MR_MUHLIK,
	AGENT_T_PHOENIX = 5200,
	AGENT_T_PHOENIX_A,
	AGENT_T_PHOENIX_B,
	AGENT_T_PHOENIX_C,
	AGENT_T_PHOENIX_D,
	AGENT_T_PHOENIX_SOLDIER,
	AGENT_T_PHOENIX_ENFORCER,
	AGENT_T_PHOENIX_SLINGSHOT,
	AGENT_CT_FBI = 5300,
	AGENT_CT_FBI_A,
	AGENT_CT_FBI_C,
	AGENT_CT_FBI_D,
	AGENT_CT_FBI_E,
	AGENT_CT_FBI_SWAT_OPERATOR,
	AGENT_CT_FBI_HRT_MARKUS_DELROW,
	AGENT_CT_FBI_SNIPER_MICHAEL_SYFERS,
	AGENT_CT_FBI_SPECIAL_AGENT_AVA,
	AGENT_CT_KSK_3RD_COMMANDO_COMPANY = 5400,
	AGENT_CT_NSWC_SEAL_TEAM_6_SOLDIER,
	AGENT_CT_NSWC_SEAL_BUCKSHOT,
	AGENT_CT_USAF_TACP_TWO_TIMES_MCCOY,
	AGENT_CT_NSWC_SEAL_LT_COMMANDER_RICKSAW,
	AGENT_T_SABRE_DRAGOMIR = 5500,
	AGENT_T_SABRE_MAXIMUS,
	AGENT_T_SABRE_REZAN_THE_READY,
	AGENT_T_SABRE_BLACKWOLF,
	AGENT_T_SABRE_THE_DOCTOR_ROMANOV,
	AGENT_CT_SAS = 5600,
	AGENT_CT_SAS_B_SQUADRON_OFFICER
};

enum e_move_type {
	MOVE_TYPE_NONE,
	MOVE_TYPE_ISOMETRIC,
	MOVE_TYPE_WALK,
	MOVE_TYPE_STEP,
	MOVE_TYPE_FLY,
	MOVE_TYPE_FLYGRAVITY,
	MOVE_TYPE_VPHYSICS,
	MOVE_TYPE_PUSH,
	MOVE_TYPE_NOCLIP,
	MOVE_TYPE_LADDER,
	MOVE_TYPE_OBSERVER,
	MOVE_TYPE_CUSTOM
};

enum e_entity_flags {
	FL_ONGROUND			= (1 << 0),
	FL_DUCKING			= (1 << 1),
	FL_DUCKING_ANIM		= (1 << 2),
	FL_WATER_JUMP		= (1 << 3),
	FL_ON_TRAIN			= (1 << 4),
	FL_IN_RAIN			= (1 << 5),
	FL_FROZEN			= (1 << 6),
	FL_AT_CONTROLS		= (1 << 7),
	FL_CLIENT			= (1 << 8),
	FL_FAKE_CLIENT		= (1 << 9),
	FL_IN_WATER			= (1 << 10),
	FL_FLY				= (1 << 11),
	FL_SWIM				= (1 << 12),
	FL_CONVEYOR			= (1 << 13),
	FL_NPC				= (1 << 14),
	FL_GODMODE			= (1 << 15),
	FL_NO_TARGET		= (1 << 16),
	FL_AIM_TARGET		= (1 << 17),
	FL_PARTIAL_GROUND	= (1 << 18),
	FL_STATIC_PROP		= (1 << 19),
	FL_GRAPHED			= (1 << 20),
	FL_GRENADE			= (1 << 21),
	FL_STEP_MOVEMENT	= (1 << 22),
	FL_DONT_TOUCH		= (1 << 23),
	FL_BASE_VELOCITY	= (1 << 24),
	FL_WORLD_BRUSH		= (1 << 25),
	FL_OBJECT			= (1 << 26),
	FL_KILL_ME			= (1 << 27),
	FL_ON_FIRE			= (1 << 28),
	FL_DISSOLVING		= (1 << 29),
	FL_RAGDOLL			= (1 << 30),
	FL_UNBLOCKABLE		= (1 << 31)
};

enum e_life_state {
	LIFE_ALIVE,
	LIFE_DYING,
	LIFE_DEAD,
	LIFE_RESPAWNABLE,
	LIFE_DISCARDBODY
};

enum e_observer_mode {
	OBS_MODE_NONE,
	OBS_MODE_DEATHCAM,
	OBS_MODE_FREEZECAM,
	OBS_MODE_FIXED,
	OBS_MODE_IN_EYE,
	OBS_MODE_CHASE,
	OBS_MODE_POI,
	OBS_MODE_ROAMING
};

enum e_team_id {
	TEAM_UNASSIGNED,
	TEAM_SPECTATOR,
	TEAM_TERRORIST,
	TEAM_COUNTER_TERRORIST
};

enum e_anim_state_poses {
	ACT_DIE_STAND = 953,
	ACT_DIE_STAND_HEADSHOT,
	ACT_DIE_CROUCH,
	ACT_DIE_CROUCH_HEADSHOT,
	ACT_NULL,
	ACT_DEFUSE,
	ACT_DEFUSE_WITH_KIT,
	ACT_FLASHBANG_REACTION,
	ACT_FIRE_PRIMARY,
	ACT_FIRE_PRIMARY_OPT_1,
	ACT_FIRE_PRIMARY_OPT_2,
	ACT_FIRE_SECONDARY,
	ACT_FIRE_SECONDARY_OPT_1,
	ACT_FIRE_SECONDARY_OPT_2,
	ACT_RELOAD,
	ACT_RELOAD_START,
	ACT_RELOAD_LOOP,
	ACT_RELOAD_END,
	ACT_OPERATE,
	ACT_DEPLOY,
	ACT_CATCH,
	ACT_SILENCER_DETACH,
	ACT_SILENCER_ATTACH,
	ACT_TWITCH,
	ACT_TWITCH_BUYZONE,
	ACT_PLANT_BOMB,
	ACT_IDLE_ADJUST_TURN,
	ACT_IDLE_ADJUST_STOPPEDMOVING,
	ACT_ALIVE_LOOP,
	ACT_FLINCH,
	ACT_FLINCH_HEAD,
	ACT_FLINCH_MOLOTOV,
	ACT_JUMP,
	ACT_FALL,
	ACT_CLIMB_LADDER,
	ACT_LAND_LIGHT,
	ACT_LAND_HEAVY,
	ACT_EXIT_LADDER_TOP,
	ACT_EXIT_LADDER_BOTTOM
};

enum e_client_frame_stage {
	FRAME_UNDEFINED = -1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,
	FRAME_RENDER_START,
	FRAME_RENDER_END
};

enum e_override_type {
	OVERRIDE_NORMAL,
	OVERRIDE_BUILD_SHADOWS,
	OVERRIDE_DEPTH_WRITE,
	OVERRIDE_SSAO_DEPTH_WRITE,
};

enum e_trace_type {
	TRACE_EVERYTHING,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS
};

enum e_anim_layer {
	ANIMATION_LAYER_AIMMATRIX,
	ANIMATION_LAYER_WEAPON_ACTION,
	ANIMATION_LAYER_WEAPON_ACTION_RECROUCH,
	ANIMATION_LAYER_ADJUST,
	ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL,
	ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB,
	ANIMATION_LAYER_MOVEMENT_MOVE,
	ANIMATION_LAYER_MOVEMENT_STRAFECHANGE,
	ANIMATION_LAYER_WHOLE_BODY,
	ANIMATION_LAYER_FLASHED,
	ANIMATION_LAYER_FLINCH,
	ANIMATION_LAYER_ALIVELOOP,
	ANIMATION_LAYER_LEAN
};

enum e_pose_param {
	POSE_PARAM_STRAFE_YAW,
	POSE_PARAM_STAND,
	POSE_PARAM_LEAN_YAW,
	POSE_PARAM_SPEED,
	POSE_PARAM_LADDER_YAW,
	POSE_PARAM_LADDER_SPEED,
	POSE_PARAM_JUMP_FALL,
	POSE_PARAM_MOVE_YAW,
	POSE_PARAM_MOVE_BLEND_CROUCH,
	POSE_PARAM_MOVE_BLEND_WALK,
	POSE_PARAM_MOVE_BLEND_RUN,
	POSE_PARAM_BODY_YAW,
	POSE_PARAM_BODY_PITCH,
	POSE_PARAM_AIM_BLEND_STAND_IDLE,
	POSE_PARAM_AIM_BLEND_STAND_WALK,
	POSE_PARAM_AIM_BLEND_STAND_RUN,
	POSE_PARAM_AIM_BLEND_COURCH_IDLE,
	POSE_PARAM_AIM_BLEND_CROUCH_WALK,
	POSE_PARAM_DEATH_YAW
};

enum e_collision_group {
	COLLISION_GROUP_NONE,
	COLLISION_GROUP_DEBRIS,
	COLLISION_GROUP_DEBRIS_TRIGGER,
	COLLISION_GROUP_INTERACTIVE_DEBRIS,
	COLLISION_GROUP_INTERACTIVE,
	COLLISION_GROUP_PLAYER,
	COLLISION_GROUP_BREAKABLE_GLASS,
	COLLISION_GROUP_VEHICLE,
	COLLISION_GROUP_PLAYER_MOVEMENT,		
	COLLISION_GROUP_NPC,
	COLLISION_GROUP_IN_VEHICLE,
	COLLISION_GROUP_WEAPON,
	COLLISION_GROUP_VEHICLE_CLIP,
	COLLISION_GROUP_PROJECTILE,
	COLLISION_GROUP_DOOR_BLOCKER,
	COLLISION_GROUP_PASSABLE_DOOR,
	COLLISION_GROUP_DISSOLVING,
	COLLISION_GROUP_PUSHAWAY,
	COLLISION_GROUP_NPC_ACTOR,
	COLLISION_GROUP_NPC_SCRIPTED,
	LAST_SHARED_COLLISION_GROUP
};

enum e_effects_type {
	EF_BONE_MERGE			= (1 << 0),
	EF_BRIGHT_LIGHT			= (1 << 1),
	EF_DIMLIGHT				= (1 << 2),
	EF_NO_INTERP			= (1 << 3),
	EF_NO_SHADOW			= (1 << 4),
	EF_NO_DRAW				= (1 << 5),
	EF_NO_RECEIVE_SHADOW	= (1 << 6),
	EF_BONEMERGE_FASTCULL	= (1 << 7),
	EF_ITEM_BLINK			= (1 << 8),
	EF_PARENT_ANIMATES		= (1 << 9),
	EF_FAST_REFLECTION		= (1 << 10),
	EF_NO_SHADOWDEPTH		= (1 << 11),
	EF_NO_SHADOWDEPTH_CACHE = (1 << 12),
	EF_NO_FLASHLIGHT		= (1 << 13),
	EF_NO_CSM				= (1 << 14)
};

enum e_eflags_type {
	EFL_KILLME 								= (1 << 0),
	EFL_DORMANT 							= (1 << 1),
	EFL_NOCLIP_ACTIVE 						= (1 << 2),
	EFL_SETTING_UP_BONES 					= (1 << 3),
	EFL_KEEP_ON_RECREATE_ENTITIES			= (1 << 4),
	EFL_HAS_PLAYER_CHILD 					= (1 << 4),
	EFL_DIRTY_SHADOWUPDATE 					= (1 << 5),
	EFL_NOTIFY 								= (1 << 6),
	EFL_FORCE_CHECK_TRANSMIT 				= (1 << 7),
	EFL_BOT_FROZEN 							= (1 << 8),
	EFL_SERVER_ONLY 						= (1 << 9),
	EFL_NO_AUTO_EDICT_ATTACH 				= (1 << 10),
	EFL_DIRTY_ABSTRANSFORM 					= (1 << 11),
	EFL_DIRTY_ABSVELOCITY 					= (1 << 12),
	EFL_DIRTY_ABSANGVELOCITY 				= (1 << 13),
	EFL_DIRTY_SURROUNDING_COLLISION_BOUNDS 	= (1 << 14),
	EFL_DIRTY_SPATIAL_PARTITION 			= (1 << 15),
	EFL_IN_SKYBOX 							= (1 << 17),
	EFL_USE_PARTITION_WHEN_NOT_SOLID 		= (1 << 18),
	EFL_TOUCHING_FLUID 						= (1 << 19),
	EFL_IS_BEING_LIFTED_BY_BARNACLE 		= (1 << 20),
	EFL_NO_ROTORWASH_PUSH 					= (1 << 21),
	EFL_NO_THINK_FUNCTION 					= (1 << 22),
	EFL_NO_GAME_PHYSICS_SIMULATION 			= (1 << 23),
	EFL_CHECK_UNTOUCH 						= (1 << 24),
	EFL_DONTBLOCKLOS 						= (1 << 25),
	EFL_DONTWALKON 							= (1 << 26),
	EFL_NO_DISSOLVE 						= (1 << 27),
	EFL_NO_MEGAPHYSCANNON_RAGDOLL 			= (1 << 28),
	EFL_NO_WATER_VELOCITY_CHANGE 			= (1 << 29),
	EFL_NO_PHYSCANNON_INTERACTION 			= (1 << 30),
	EFL_NO_DAMAGE_FORCES 					= (1 << 31)
};

enum e_bone_flags {
	BF_PHYSICALLY_SIMULATED		= (1 << 0),
	BF_PHYSICS_PROCEDURAL		= (1 << 1),
	BF_ALWAYS_PROCEDURAL		= (1 << 2),
	BF_SCREEN_ALIGN_SPHERE		= (1 << 3),
	BF_SCREEN_ALIGN_CYLINDER	= (1 << 4),
	BF_CALCULATE_MASK			= (1 << 5) - BF_PHYSICALLY_SIMULATED,
	BF_USED_BY_HITBOX			= (1 << 8),
	BF_USED_BY_ATTACHMENT		= (1 << 9),
	BF_USED_BY_VERTEX_LOD0		= (1 << 10),
	BF_USED_BY_VERTEX_LOD1		= (1 << 11),
	BF_USED_BY_VERTEX_LOD2		= (1 << 12),
	BF_USED_BY_VERTEX_LOD3		= (1 << 13),
	BF_USED_BY_VERTEX_LOD4		= (1 << 14),
	BF_USED_BY_VERTEX_LOD5		= (1 << 15),
	BF_USED_BY_VERTEX_LOD6		= (1 << 16),
	BF_USED_BY_VERTEX_LOD7		= (1 << 17),
	BF_USED_BY_BONE_MERGE		= (1 << 18),
	BF_USED_BY_VERTEX_MASK		= (BF_USED_BY_BONE_MERGE - BF_USED_BY_VERTEX_LOD0),
	BF_USED_BY_ANYTHING			= (1 << 19) - BF_USED_BY_HITBOX,
	BF_USED_MASK				= (1 << 19) - BF_USED_BY_HITBOX,
	BF_FIXED_ALIGNMENT			= (1 << 20),
	BF_HAS_SAVEFRAME_POS		= (1 << 21),
	BF_HAS_SAVEFRAME_ROT64		= (1 << 22),
	BF_HAS_SAVEFRAME_ROT32		= (1 << 23),
	BF_TYPE_MASK				= 0x00F00000
};

enum e_hitgroups {
	HITGROUP_GENERIC,
	HITGROUP_HEAD,
	HITGROUP_CHEST,
	HITGROUP_STOMACH,
	HITGROUP_LEFT_ARM,
	HITGROUP_RIGHT_ARM,
	HITGROUP_LEFT_LEG,
	HITGROUP_RIGHT_LEG,
	HITGROUP_GEAR = 10
};

enum e_hitboxes {
	HITBOX_HEAD,
	HITBOX_NECK,
	HITBOX_PELVIS,
	HITBOX_STOMACH,
	HITBOX_LOWER_CHEST,
	HITBOX_CHEST,
	HITBOX_UPPER_CHEST,
	HITBOX_RIGHT_THIGH,
	HITBOX_LEFT_THIGH,
	HITBOX_RIGHT_CALF,
	HITBOX_LEFT_CALF,
	HITBOX_RIGHT_FOOT,
	HITBOX_LEFT_FOOT,
	HITBOX_RIGHT_HAND,
	HITBOX_LEFT_HAND,
	HITBOX_RIGHT_UPPER_ARM,
	HITBOX_RIGHT_FOREARM,
	HITBOX_LEFT_UPPER_ARM,
	HITBOX_LEFT_FOREARM
};

enum e_solid_type {
	SOLID_NONE,
	SOLID_BSP,
	SOLID_BBOX,
	SOLID_OBB,
	SOLID_OBB_YAW,
	SOLID_CUSTOM,
	SOLID_VPHYSICS,
	SOLID_LAST
};

enum e_studio_flags {
	STUDIO_NONE									= 0,
	STUDIO_RENDER								= (1 << 0),
	STUDIO_VIEW_XFORM_ATTACHMENTS			= (1 << 1),
	STUDIO_DRAW_TRANSLUCENT_SUBMODELS	= (1 << 2),
	STUDIO_TWO_PASS							= (1 << 3),
	STUDIO_STATIC_LIGHTING					= (1 << 4),
	STUDIO_WIREFRAME							= (1 << 5),
	STUDIO_ITEM_BLINK							= (1 << 6),
	STUDIO_NOSHADOWS							= (1 << 7),
	STUDIO_WIREFRAME_VCOLLIDE				= (1 << 8),
	STUDIO_NOLIGHTING_OR_CUBEMAP			= (1 << 9),
	STUDIO_SKIP_FLEXES						= (1 << 10),
	STUDIO_DONT_MODIFY_STENCIL_STATE		= (1 << 11),
	STUDIO_DEPTH_ONLY							= (1 << 24),
	STUDIO_AO_PREPASS_TEXTURE				= (1 << 25),
	STUDIO_KEEP_SHADOWS						= (1 << 26),
	STUDIO_SSAO_DEPTH_TEXTURE				= (1 << 27),
	STUDIO_SKIP_DECALS						= (1 << 28),
	STUDIO_SHADOW_TEXTURE					= (1 << 29),
	STUDIO_SHADOW_DEPTH_TEXTURE			= (1 << 30),
	STUDIO_TRANSPARENCY						= (1 << 31),
};

enum e_flow_type {
	FLOW_OUTGOING,
	FLOW_INCOMING,
	MAX_FLOWS
};

enum e_damage_type {
	DAMAGE_NO,
	DAMAGE_EVENTS_ONLY,
	DAMAGE_YES,
	DAMAGE_AIM
};

enum e_studio_proc_type {
	STUDIO_PROC_AXISINTERP = 1,
	STUDIO_PROC_QUATINTERP,
	STUDIO_PROC_AIMATBONE,
	STUDIO_PROC_AIMATATTACH,
	STUDIO_PROC_JIGGLE,
	STUDIO_PROC_TWIST_MASTER,
	STUDIO_PROC_TWIST_SLAVE,
};

enum e_surf_flags {
	SURF_LIGHT		= (1 << 0),
	SURF_SKY2D		= (1 << 1),
	SURF_SKY			= (1 << 2),
	SURF_WARP		= (1 << 3),
	SURF_TRANS		= (1 << 4),
	SURF_NOPORTAL	= (1 << 5),
	SURF_TRIGGER	= (1 << 6),
	SURF_NODRAW		= (1 << 7),
	SURF_HINT		= (1 << 8),
	SURF_SKIP		= (1 << 9),
	SURF_NOLIGHT	= (1 << 10),
	SURF_BUMPLIGHT = (1 << 11),
	SURF_NOSHADOWS = (1 << 12),
	SURF_NODECALS	= (1 << 13),
	SURF_NOCHOP		= (1 << 14),
	SURF_HITBOX		= (1 << 15),
	SURF_NOPAINT	= SURF_NODECALS
};

enum e_buttons {
	IN_ATTACK      = (1 << 0),
	IN_JUMP        = (1 << 1),
	IN_DUCK        = (1 << 2),
	IN_FORWARD     = (1 << 3),
	IN_BACK        = (1 << 4),
	IN_USE         = (1 << 5),
	IN_CANCEL      = (1 << 6),
	IN_LEFT        = (1 << 7),
	IN_RIGHT       = (1 << 8),
	IN_MOVELEFT    = (1 << 9),
	IN_MOVERIGHT   = (1 << 10),
	IN_ATTACK2     = (1 << 11),
	IN_RUN         = (1 << 12),
	IN_RELOAD      = (1 << 13),
	IN_ALT1        = (1 << 14),
	IN_ALT2        = (1 << 15),
	IN_SCORE       = (1 << 16),
	IN_SPEED       = (1 << 17),
	IN_WALK        = (1 << 18),
	IN_ZOOM        = (1 << 19),
	IN_WEAPON1     = (1 << 20),
	IN_WEAPON2     = (1 << 21),
	IN_BULLRUSH    = (1 << 22),
	IN_GRENADE1    = (1 << 23),
	IN_GRENADE2    = (1 << 24),
	IN_SPINLOOK    = (1 << 25)
};

enum e_beam_type {
	TE_BEAMPOINTS,
	TE_SPRITE,
	TE_BEAMDISK,
	TE_BEAMCYLINDER,
	TE_BEAMFOLLOW,
	TE_BEAMRING,
	TE_BEAMSPLINE,
	TE_BEAMRINGPOINT,
	TE_BEAMLASER,
	TE_BEAMTESLA
};

enum e_beam_flags {
	FBEAM_STARTENTITY		= (1 << 0),
	FBEAM_ENDENTITY		= (1 << 1),
	FBEAM_FADEIN			= (1 << 2),
	FBEAM_FADEOUT			= (1 << 3),
	FBEAM_SINENOISE		= (1 << 4),
	FBEAM_SOLID				= (1 << 5),
	FBEAM_SHADEIN			= (1 << 6),
	FBEAM_SHADEOUT			= (1 << 7),
	FBEAM_ONLYNOISEONCE	= (1 << 8),
	FBEAM_NOTILE			= (1 << 9),
	FBEAM_USE_HITBOXES	= (1 << 10),
	FBEAM_STARTVISIBLE	= (1 << 11),
	FBEAM_ENDVISIBLE		= (1 << 12),
	FBEAM_ISACTIVE			= (1 << 13),
	FBEAM_FOREVER			= (1 << 14),
	FBEAM_HALOBEAM			= (1 << 15),
	FBEAM_REVERSED			= (1 << 16)
};

enum e_game_type {
	GAMETYPE_UNKNOWN = -1,
	GAMETYPE_CLASSIC,
	GAMETYPE_GUNGAME,
	GAMETYPE_TRAINING,
	GAMETYPE_CUSTOM,
	GAMETYPE_COOPERATIVE,
	GAMETYPE_SKIRMISH,
	GAMETYPE_FREEFORALL
};

enum e_game_mode {
	GAMEMODE_UNKNOWN = 0,
	GAMEMODE_CASUAL,
	GAMEMODE_COMPETITIVE,
	GAMEMODE_WINGMAN,
	GAMEMODE_ARMSRACE,
	GAMEMODE_DEMOLITION,
	GAMEMODE_DEATHMATCH,
	GAMEMODE_GUARDIAN,
	GAMEMODE_COOPSTRIKE,
	GAMEMODE_DANGERZONE
};
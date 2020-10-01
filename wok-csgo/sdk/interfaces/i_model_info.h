#pragma once

struct anim_tag_t {
	int		m_index;
	float	m_first_cycle;
	int		m_tag_string_offset_from_this;

	const char* get_name() { return (const char*)this + m_tag_string_offset_from_this; }
};

struct mstudiobbox_t {
	int         m_bone;
	int         m_group;
	vec3_t      m_obb_min;
	vec3_t      m_obb_max;
	int         m_hitbox_name_index;
	angle_t	m_rotation;
	float       m_radius;
	char		pad1[16];

	__forceinline const char* get_name() const {
		if (!m_hitbox_name_index)
			return nullptr;

		return (const char*)((uint8_t*)this + m_hitbox_name_index);
	}

	__forceinline vec3_t get_position(const matrix3x4_t* bones) const { return ((m_obb_min + m_obb_max) / 2.f).transform(bones[m_bone]); }
};

struct mstudiohitboxset_t {
	int    m_name_index;
	int    m_num_hitboxes;
	int    m_hitbox_index;

	__forceinline const char* get_name() {
		if (!m_name_index)
			return nullptr;

		return (const char*)((uint8_t*)this + m_name_index);
	}

	__forceinline mstudiobbox_t* get_hitbox(int i) {
		if (i > m_num_hitboxes)
			return nullptr;

		return (mstudiobbox_t*)((uint8_t*)this + m_hitbox_index) + i;
	}
};

struct mstudiobone_t {
	int						m_name_index;
	int						m_parent;
	int						m_bone_controller[6];
	vec3_t					m_pos;
	vec4_t					m_quat;
	vec3_t					m_rot;

	vec3_t					m_pos_scale;
	vec3_t					m_rot_scale;

	matrix3x4_t				m_pose_to_bone;
	vec4_t					m_alignment;
	bit_flag_t				m_flags;
	int						m_proc_type;
	int						m_proc_index;
	mutable int				m_physics_bone;
	int						m_surface_prop_index;

	int						m_contents;
	int						m_surface_prop_lookup;
	char					pad0[28];

	__forceinline char* const get_name() const { return ((char*)this) + m_name_index; }
	__forceinline void*	get_procedure() const { if (m_proc_index == 0) return 0; else return (void*)(((byte*)this) + m_proc_index); };
	__forceinline char* const get_sz_surface_prop() const { return ((char*)this) + m_surface_prop_index; }
	__forceinline int get_surface_prop() const { return m_surface_prop_lookup; }
};

struct mstudioposeparamdesc_t {
	int					m_name_index;
	bit_flag_t			m_flags;
	float				m_start;
	float				m_end;
	float				m_loop;

	__forceinline const char* get_name() const { return ((char*)this) + m_name_index; }
};

struct mstudioactivitymodifier_t {
	int					m_name_index;

	__forceinline const char* get_name() const { return m_name_index ? (char*)(((byte*)this) + m_name_index) : nullptr; }
};

struct mstudioseqdesc_t {
	char				pad0[4];
	int					m_label_index;
	int					m_activity_name_index;
	bit_flag_t			m_flags;
	int					m_activity;
	int					m_activity_weight;
	int					m_num_events;
	int					m_event_index;
	vec3_t				m_obb_min;
	vec3_t				m_obb_max;
	int					m_num_blends;
	int					m_anim_index;
	int					m_movement_index;
	int					m_group_size[2];
	int					m_param_index[2];
	float				m_param_start[2];
	float				m_param_end[2];
	int					m_param_parent;
	float				m_fade_in_time;
	float				m_fade_out_time;
	int					m_local_entry_node;
	int					m_local_exit_node;
	bit_flag_t			m_node_flags;
	float				m_entry_phase;
	float				m_exit_phase;
	float				m_last_frame;
	int					m_next_seq;
	int					m_pose;
	int					m_num_ik_rules;
	int					m_num_auto_layers;
	int					m_auto_layer_index;
	int					m_weight_list_index;
	int					m_pose_key_index;
	int					m_num_ik_locks;
	int					m_ik_lock_index;
	int					m_key_value_index;
	int					m_key_value_size;
	int					m_cycle_pose_index;
	int					m_activity_modifier_index;
	int					m_num_activity_modifiers;
	int					m_anim_tag_index;
	int					m_num_anim_tags;
	char				pad1[12];

	__forceinline anim_tag_t* get_anim_tag(int i) const { return (anim_tag_t*)(((byte*)this) + m_anim_tag_index) + i; }

	__forceinline mstudioactivitymodifier_t* get_activity_modifier(int i) const { return m_activity_modifier_index != 0 ? (mstudioactivitymodifier_t*)(((byte*)this) + m_activity_modifier_index) + i : nullptr; };
};

class studiohdr_t {
public:
	int				m_id;
	int				m_version;
	long			m_checksum;
	char			m_name[64];
	int				m_length;
	vec3_t			m_eye_pos;
	vec3_t			m_illum_pos;
	vec3_t			m_hull_min;
	vec3_t			m_hull_max;
	vec3_t			m_obb_min;
	vec3_t			m_obb_max;
	bit_flag_t		m_flags;
	int				m_num_bones;
	int				m_bone_index;
	int				m_num_bone_controllers;
	int				m_bone_controller_index;
	int				m_num_hitbox_sets;
	int				m_hitbox_set_index;
	int				m_num_local_anim;
	int				m_local_anim_index;
	int				m_num_local_seq;
	int				m_local_seq_index;
	int				m_activity_list_version;
	int				m_events_indexed;
	int				m_num_textures;
	int				m_texture_index;
	int				m_num_cd_textures;
	int				m_cd_texture_index;
	int				m_num_skin_ref;
	int				m_num_skin_families;
	int				m_skin_index;
	int				m_num_body_parts;
	int				m_body_part_index;
	int				m_num_local_attachments;
	int				m_local_attachment_index;
	int				m_num_local_nodes;
	int				m_local_node_index;
	int				m_local_node_name_index;
	int				m_num_flex_desc;
	int				m_flex_desc_index;
	int				m_num_flex_controllers;
	int				m_flex_controller_index;
	int				m_num_flex_rules;
	int				m_flex_rule_index;
	int				m_num_ik_chains;
	int				m_ik_chain_index;
	int				m_num_mouths;
	int				m_mouth_index;
	int				m_num_local_pose_parameters;
	int				m_local_pose_param_index;
	int				m_surface_prop_index;
	int				m_key_value_index;
	int				m_key_value_size;
	int				m_num_local_ik_auto_play_locks;
	int				m_local_ik_auto_play_lock_index;
	float			m_mass;
	int				m_contents;
	int				m_num_include_models;
	int				m_include_model_index;
	mutable void*	m_virtual_model;
	int				m_anim_block_name_index;
	int				m_num_anim_blocks;
	int				m_anim_block_index;
	mutable void*	m_anim_block_model;
	int				m_bone_table_by_name_index;
	void*			m_vertex_base;
	void*			m_index_base;
	uint8_t			m_const_directional_light_dot;
	uint8_t			m_root_lod;
	uint8_t			m_num_allowed_root_lods;
	char			pad0[5];
	int				m_num_flex_controller_ui;
	int				m_flex_controller_ui_index;
	float			m_vert_anim_fixed_point_scale;
	mutable int		m_surface_prop_lookup;
	int				m_studio_hdr_index;
	char			pad1[4];

	__forceinline mstudiohitboxset_t* get_hitbox_set(int i) const { return i <= m_num_hitbox_sets ? (mstudiohitboxset_t*)((uint8_t*)this + m_hitbox_set_index) + i : nullptr; }

	__forceinline mstudiobone_t* get_bone(int i) const { return i <= m_num_bones ? (mstudiobone_t*)((uint8_t*)this + m_bone_index) + i : nullptr; }

	__forceinline mstudioseqdesc_t* get_local_seqdesc(int i) const {
		if (i < 0 || i >= m_num_local_seq)
			i = 0;

		return (mstudioseqdesc_t*)((char*)this + sizeof(mstudioseqdesc_t) * i + m_local_seq_index);
	};

	__forceinline mstudioposeparamdesc_t* get_local_pose_parameter(int i) const { return (mstudioposeparamdesc_t*)(((byte*)this) + m_local_pose_param_index) + i; };
};

class virtualgroup_t {
public:
	void*				m_cache;

	__forceinline studiohdr_t* get_studio_hdr() const { return (studiohdr_t*)m_cache; }

	c_utl_vector<int>	m_bone_map;
	c_utl_vector<int>	m_master_bone;
	c_utl_vector<int>	m_master_seq;
	c_utl_vector<int>	m_master_anim;
	c_utl_vector<int>	m_master_attachment;
	c_utl_vector<int>	m_master_pose;
	c_utl_vector<int>	m_master_node;
};

struct virtualsequence_t {
	bit_flag_t	m_flags;
	int			m_activity;
	int			m_group;
	int			m_index;
};

struct virtualgeneric_t {
	int m_group;
	int m_index;
};

struct virtualmodel_t {
	char							pad0[8];
	c_utl_vector<virtualsequence_t>	m_seq;
	c_utl_vector<virtualgeneric_t>	m_anim;
	c_utl_vector<virtualgeneric_t>	m_attachment;
	c_utl_vector<virtualgeneric_t>	m_pose;
	c_utl_vector<virtualgroup_t>	m_group;
	c_utl_vector<virtualgeneric_t>	m_node;
	c_utl_vector<virtualgeneric_t>	m_ik_lock;
	c_utl_vector<uint16_t>			m_auto_play_sequences;
};

class c_activity_to_sequence_mapping {
public:
	char pad0[32];

	friend class c_studio_hdr;
};

class c_studio_hdr {
public:
	mutable studiohdr_t*						m_studio_hdr;
	mutable virtualmodel_t*						m_v_model;
	char										pad0[4];
	mutable c_utl_vector<const studiohdr_t*>	m_studio_hdr_cache;
	mutable int									m_num_frame_unlock_counter;
	int*										m_frame_unlock_counter;
	char										pad1[8];
	c_utl_vector<bit_flag_t>					m_bone_flags;
	c_utl_vector<int>							m_bone_parent;
	c_activity_to_sequence_mapping*				m_activity_to_sequence;

	VFUNC_SIG(group_studio_hdr(int group), "client.dll", "55 8B EC 56 8B F1 57 85 F6", const studiohdr_t*(__thiscall*)(void*, int), group)
	VFUNC_SIG(find_mapping(), "client.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 57 8B F9 8B 17 83 BA ? ? ? ? ? 74 34", c_activity_to_sequence_mapping*(__thiscall*)(void*))

	__forceinline const mstudioposeparamdesc_t& get_pose_parameter(int i) {
		if (!m_v_model) {
			return *m_studio_hdr->get_local_pose_parameter(i);
		}

		auto pose = &m_v_model->m_pose.element(i);

		if (pose->m_group == 0)
			return *m_studio_hdr->get_local_pose_parameter(pose->m_index);

		const auto hdr = group_studio_hdr(pose->m_group);

		return *hdr->get_local_pose_parameter(pose->m_index);
	}

	int	get_num_pose_params() const { return m_v_model ? m_v_model->m_pose.count() : m_studio_hdr->m_num_local_pose_parameters; }
};

class i_model_info {
public:
	VFUNC(get_model(int index), 1, model_t*(__thiscall*)(void*, int), index)
		VFUNC(get_model_index(const char* name), 2, int(__thiscall*)(void*, const char*), name)
		VFUNC(get_model_name(const model_t* model), 3, const char*(__thiscall*)(void*, const model_t*), model)
		VFUNC(get_studio_model(const model_t* model), 32, studiohdr_t*(__thiscall*)(void*, const model_t*), model)
};
#pragma once

struct surfacephysicsparams_t {
	float    m_friction;
	float    m_elasticity;
	float    m_density;
	float    m_thickness;
	float    m_dampening;
};

struct surfaceaudioparams_t {
	float    m_reflectivity;
	float    m_hardness_factor;
	float    m_roughness_factor;
	float    m_rough_threshold;
	float    m_hard_threshold;
	float    m_hard_velocity_threshold;
	float    m_high_pitch_occlusion;
	float    m_mid_pitch_occlusion;
	float    m_low_pitch_occlusion;
};

struct surfacesoundnames_t {
	uint16_t    m_walk_step_left;
	uint16_t    m_walk_step_right;
	uint16_t	m_run_step_left;
	uint16_t	m_run_step_right;
	uint16_t    m_impact_soft;
	uint16_t    m_impact_hard;
	uint16_t    m_scrape_smooth;
	uint16_t    m_scrape_rough;
	uint16_t    m_bullet_impact;
	uint16_t    m_rolling;
	uint16_t    m_break_sound;
	uint16_t    m_strain_sound;
};

struct surfacegameprops_t {
	float		m_max_speed_factor;
	float		m_jump_factor;
	float		m_penetration_modifier;
	float		m_damage_modifier;
	uint16_t	m_material;
	uint8_t		m_climbable;
};

struct surfacedata_t {
	surfacephysicsparams_t	m_physics;
	surfaceaudioparams_t	m_audio;
	surfacesoundnames_t		m_sounds;
	surfacegameprops_t		m_game;
	char					pad[48];
};

class i_surface_data {
public:
	virtual					~i_surface_data() = default;
	virtual int				parse_surface_data(const char* filename, const char* textfile) = 0;
	virtual int				surface_prop_count() const = 0;
	virtual int				get_surface_index(const char* name) const = 0;
	virtual void			get_physics_properties(int index, float* density, float* thickness, float* friction, float* elasticity) const = 0;
	virtual surfacedata_t*	get_surface_data(int index) = 0;
	virtual const char*		get_string(unsigned short index) const = 0;
	virtual const char*		get_prop_name(int index) const = 0;
	virtual void			set_world_material_index_table(int* map, int size) = 0;
	virtual void			get_physics_parameters(int index, surfacephysicsparams_t* out) const = 0;
};

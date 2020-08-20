#pragma once

enum e_damage_type {
	DAMAGE_NO,
	DAMAGE_EVENTS_ONLY,
	DAMAGE_YES,
	DAMAGE_AIM
};

class i_engine_trace {
public:
	virtual int	get_point_contents(const vec3_t& pos, int mask = MASK_ALL, i_handle_entity** entity = 0) = 0;
	virtual int	get_point_contents_world_only(const vec3_t& pos, int mask = MASK_ALL) = 0;
	virtual int	get_point_contents_collideable(i_collideable* collideable, const vec3_t& pos) = 0;
	virtual void clip_ray_to_entity(const ray_t& ray, unsigned int mask, i_handle_entity* entity, c_game_trace* trace) = 0;
	virtual void clip_ray_to_collideable(const ray_t& ray, unsigned int mask, i_collideable* collideable, c_game_trace* trace) = 0;
	virtual void trace_ray(const ray_t& ray, unsigned int mask, i_trace_filter* filter, c_game_trace* trace) = 0;
};

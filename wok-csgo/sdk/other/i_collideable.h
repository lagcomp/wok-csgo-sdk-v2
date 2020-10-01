#pragma once

struct ray_t;
struct model_t;

class c_game_trace;

class i_handle_entity;
class i_client_unknown;

class i_collideable {
public:
	virtual i_handle_entity*    get_entity_handle() = 0;
	virtual vec3_t&				obb_mins() const = 0;
	virtual vec3_t&				obb_maxs() const = 0;
	virtual void                world_space_trigger_bounds(vec3_t* mins, vec3_t* maxs) const = 0;
	virtual bool                test_collision(const ray_t &ray, unsigned int mask, c_game_trace& trace) = 0;
	virtual bool                test_hitboxes(const ray_t &ray, unsigned int mask, c_game_trace& trace) = 0;
	virtual int                 get_collision_model_index() = 0;
	virtual const model_t*      get_collision_model() = 0;
	virtual const vec3_t&		get_collision_origin() const = 0;
	virtual const angle_t&     get_collision_angles() const = 0;
	virtual const matrix3x4_t&	collision_to_world_transform() const = 0;
	virtual e_solid_type		get_solid() const = 0;
	virtual int                 get_solid_flags() const = 0;
	virtual i_client_unknown*   get_client_unknown() = 0;
	virtual int                 get_collision_group() const = 0;
	virtual void                world_space_surrounding_bounds(vec3_t* mins, vec3_t* maxs) = 0;
	virtual bool                should_touch_trigger(int flags) const = 0;
	virtual const matrix3x4_t*	get_root_parent_to_world_transform() const = 0;
};

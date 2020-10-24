#pragma once

class i_glow_object_manager {
public:
	struct glow_object_t {
		__forceinline void set(const col_t& clr, e_glow_style style = GLOWSTYLE_DEFAULT) {
			m_red = clr.r() / 255.f;
			m_green = clr.g() / 255.f;
			m_blue = clr.b() / 255.f;
			m_alpha = clr.a() / 255.f;

			m_render_when_occluded = true;
			m_render_when_unoccluded = false;

			m_glow_style = style;
			m_bloom_amount = 0.8f;
		}

		__forceinline bool is_unused() const { return m_next_free_slot != -2; }

		c_base_entity*		m_entity;
		float				m_red;
		float				m_green;
		float				m_blue;
		float				m_alpha;
		float				m_alpha_of_max_velocity;
		float				m_alpha_max;
		float				m_bloom_amount;
		float				m_pulse_overdrive;
		bool				m_render_invisible;
		bool				m_render_visible;
		bool				m_render_when_occluded;
		bool				m_render_when_unoccluded;
		bool				m_full_bloom_render;
		bool				m_alpha_depends_by_render_alpha;
		int					m_full_bloom_stencil_test_value;
		e_glow_style		m_glow_style;
		int					m_split_screen_slot;
		int					m_next_free_slot;
	};

	c_utl_vector<glow_object_t> m_glow_object_definitions;
	int							m_first_free_slot;
};

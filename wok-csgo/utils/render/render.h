#pragma once

enum e_font_flags {
	FONT_NONE,
	FONT_CENTERED_X = 1 << 0,
	FONT_CENTERED_Y = 1 << 1,
	FONT_CENTERED = FONT_CENTERED_X | FONT_CENTERED_Y,
	FONT_DROP_SHADOW = 1 << 2,
	FONT_OUTLINE = 1 << 4
};

namespace render {
	void init();

	void add_to_draw_list();

	void begin();

	vec2_t get_text_size(std::string_view txt, ImFont* font);

	void text(std::string_view txt, vec2_t pos, const col_t& clr, ImFont* font, bit_flag_t<uint8_t> flags = FONT_NONE);

	void line(const vec2_t& from, const vec2_t& to, const col_t& clr);

	void rect(const vec2_t& pos, const vec2_t& size, const col_t& clr, float rounding = 0.f);

	void rect_filled(const vec2_t& pos, const vec2_t& size, const col_t& clr, float rounding = 0.f);

	void rect_filled_multi_clr(const vec2_t& pos, const vec2_t& size, const col_t& clr_upr_left, const col_t& clr_upr_right, const col_t& clr_bot_left, const col_t& clr_bot_right);

	void polygon(const std::vector<vec2_t>& points, const col_t& clr);

	void polygon_filled(const std::vector<vec2_t>& points, const col_t& clr);
	
	extern std::mutex m_mutex;

	extern vec2_t m_screen_size;

	extern std::shared_ptr<ImDrawList> m_draw_list;
	extern std::shared_ptr<ImDrawList> m_data_draw_list;
	extern std::shared_ptr<ImDrawList> m_replace_draw_list;
}

namespace fonts {
	extern ImFont* m_tahoma14;
}

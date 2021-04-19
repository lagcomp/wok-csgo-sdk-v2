#include "../utils.h"
#include "../../sdk/interfaces.h"
#include "../../features/features.h"

namespace render {
	void init() {
		ImGui::CreateContext();

		ImGui_ImplWin32_Init(input::m_hwnd);
		ImGui_ImplDX9_Init(interfaces::m_d3d_device);

		m_draw_list = std::make_shared<ImDrawList>(ImGui::GetDrawListSharedData());
		m_data_draw_list = std::make_shared<ImDrawList>(ImGui::GetDrawListSharedData());
		m_replace_draw_list = std::make_shared<ImDrawList>(ImGui::GetDrawListSharedData());

		auto& io = ImGui::GetIO();
		auto& style = ImGui::GetStyle();

		style.WindowMinSize = ImVec2(500, 450);

		ImFontConfig tahoma14;
		tahoma14.RasterizerFlags = ImGuiFreeType::ForceAutoHint;
		fonts::m_tahoma14 = io.Fonts->AddFontFromFileTTF(_("C:\\Windows\\Fonts\\Tahoma.ttf"), 14.f, &tahoma14, io.Fonts->GetGlyphRangesCyrillic());

		ImGuiFreeType::BuildFontAtlas(io.Fonts);
	}

	vec2_t get_text_size(std::string_view txt, ImFont* font) {
		if (!font
			|| txt.empty()
			|| !font->IsLoaded())
			return vec2_t();

		const auto size = font->CalcTextSizeA(font->FontSize, std::numeric_limits<float>::max(), 0.f, txt.data());

		return vec2_t(IM_FLOOR(size.x + 0.95f), size.y);
	}

	void text(std::string_view txt, vec2_t pos, const col_t& clr, ImFont* font, bit_flag_t<uint8_t> flags) {
		if (!font
			|| txt.empty()
			|| clr.a() <= 0
			|| !font->IsLoaded())
			return;

		const auto centered_x = flags.has(FONT_CENTERED_X);
		const auto centered_y = flags.has(FONT_CENTERED_Y);

		if (centered_x
			|| centered_y) {
			const auto text_size = get_text_size(txt, font);

			if (centered_x) {
				pos.x -= text_size.x / 2.f;
			}

			if (centered_y) {
				pos.y -= text_size.y / 2.f;
			}
		}

		m_draw_list->PushTextureID(font->ContainerAtlas->TexID);

		if (flags.has(FONT_DROP_SHADOW)) {
			m_draw_list->AddTextSoftShadow(font, font->FontSize, *reinterpret_cast<ImVec2*>(&pos), clr.hex(), txt.data());
		}
		else if (flags.has(FONT_OUTLINE)) {
			m_draw_list->AddTextOutline(font, font->FontSize, *reinterpret_cast<ImVec2*>(&pos), clr.hex(), txt.data());
		}
		else {
			m_draw_list->AddText(font, font->FontSize, *reinterpret_cast<ImVec2*>(&pos), clr.hex(), txt.data());
		}

		m_draw_list->PopTextureID();
	}

	void line(const vec2_t& from, const vec2_t& to, const col_t& clr) {
		m_draw_list->AddLine(*reinterpret_cast<const ImVec2*>(&from), *reinterpret_cast<const ImVec2*>(&to), clr.hex());
	}

	void rect(const vec2_t& pos, const vec2_t& size, const col_t& clr, float rounding) {
		m_draw_list->AddRect(*reinterpret_cast<const ImVec2*>(&pos), ImVec2(pos.x + size.x, pos.y + size.y), clr.hex(), rounding);
	}

	void rect_filled(const vec2_t& pos, const vec2_t& size, const col_t& clr, float rounding) {
		m_draw_list->AddRectFilled(*reinterpret_cast<const ImVec2*>(&pos), ImVec2(pos.x + size.x, pos.y + size.y), clr.hex(), rounding);
	}

	void rect_filled_multi_clr(const vec2_t& pos, const vec2_t& size, const col_t& clr_upr_left, const col_t& clr_upr_right, const col_t& clr_bot_left, const col_t& clr_bot_right) {
		m_draw_list->AddRectFilledMultiColor(*reinterpret_cast<const ImVec2*>(&pos), ImVec2(pos.x + size.x, pos.y + size.y), clr_upr_left.hex(), clr_upr_right.hex(), clr_bot_right.hex(), clr_bot_left.hex());
	}

	void add_to_draw_list() {
		const auto lock = std::unique_lock<std::mutex>(m_mutex, std::try_to_lock);
		if (lock.owns_lock()) {
			*m_replace_draw_list = *m_data_draw_list;
		}

		*ImGui::GetBackgroundDrawList() = *m_replace_draw_list;
	}

	void begin() {
		m_draw_list->Clear();
		m_draw_list->PushClipRectFullScreen();

		m_screen_size = *reinterpret_cast<vec2_t*>(&ImGui::GetIO().DisplaySize);

		/* call ur visuals etc... here */

		{
			const auto lock = std::unique_lock<std::mutex>(m_mutex);

			*m_data_draw_list = *m_draw_list;
		}
	}

	void polygon(const std::vector<vec2_t>& points, const col_t& clr) {
		if (clr.a() <= 0)
			return;

		m_draw_list->_Path.reserve(m_draw_list->_Path.Size + points.size() + 1);

		for (auto& point : points) {
			m_draw_list->_Path.push_back(*reinterpret_cast<const ImVec2*>(&point));
		}

		m_draw_list->PathStroke(clr.hex(), true, 1.f);
	}

	void polygon_filled(const std::vector<vec2_t>& points, const col_t& clr) {
		if (clr.a() <= 0)
			return;

		m_draw_list->_Path.reserve(m_draw_list->_Path.Size + points.size() + 1);

		for (auto& point : points) {
			m_draw_list->_Path.push_back(*reinterpret_cast<const ImVec2*>(&point));
		}

		m_draw_list->PathFillConvex(clr.hex());
	}

	std::mutex m_mutex;

	vec2_t m_screen_size;

	std::shared_ptr<ImDrawList> m_draw_list;
	std::shared_ptr<ImDrawList> m_data_draw_list;
	std::shared_ptr<ImDrawList> m_replace_draw_list;
}

namespace fonts {
	ImFont* m_tahoma14 = nullptr;
}

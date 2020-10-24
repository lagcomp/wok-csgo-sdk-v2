#include "menu.h"

void c_menu::on_paint() {
	if (!(input::m_blocked = input::get_key<TOGGLE>(VK_INSERT)))
		return;

	ImGui::SetNextWindowPos(ImGui::GetIO().DisplaySize / 2.f, ImGuiCond_Once, ImVec2(0.5f, 0.5f));

	ImGui::SetNextWindowSize(ImVec2(500, 450), ImGuiCond_Once);

	if (ImGui::Begin(_("wok sdk v2"), 0, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse)) {
		ImGui::ColorEdit3(_("color picker 3"), cfg::get<std::vector<float>>(HASH("example::clr3")).data());
		ImGui::ColorEdit4(_("color picker 4"), cfg::get<std::vector<float>>(HASH("example::clr4")).data());

		if (ImGui::Button(_("save"))) {
			cfg::save();
		}

		if (ImGui::Button(_("load"))) {
			cfg::load();
		}
	}
	ImGui::End();
}
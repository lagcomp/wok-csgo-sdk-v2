#include "../features.h"
#include "../../hooks/hooks.h"

i_material* c_chams::create_material(const std::string& material_name, const std::string& shader_type, const std::string& material_data) {
	const auto key_values = static_cast<c_key_values*>(interfaces::mem_alloc->alloc(36u));

	key_values->init(shader_type.c_str());
	key_values->load_from_buffer(material_name.c_str(), material_data.c_str());

	return interfaces::material_system->create_material(material_name.c_str(), key_values);
}

bool c_chams::override_material(int type, const col_t& clr, bool ignorez) {
	i_material* material = nullptr;

	switch (type) {
	case M_REGULAR: material = ignorez ? m_regular_z : m_regular; break;
	case M_FLAT: material = ignorez ? m_flat_z : m_flat; break;
	case M_DONT_DRAW: return false; break;
	}

	material->alpha_modulate(clr.a() / 255.f);
	material->color_modulate(clr.r() / 255.f, clr.g() / 255.f, clr.b() / 255.f);

	if (const auto $alpha = material->find_var(_("$alpha"), nullptr, false)) {
		$alpha->set_value(clr.a() / 255.f);
	}

	if (const auto $envmaptint = material->find_var(_("$envmaptint"), nullptr, false)) {
		$envmaptint->set_value(vec3_t(clr.r() / 255.f, clr.g() / 255.f, clr.b() / 255.f));
	}

	interfaces::model_render->forced_material_override(material);

	return true;
}

bool c_chams::on_draw_model(i_model_render* ecx, void* context, const draw_model_state_t& state, const model_render_info_t& info, matrix3x4_t* bones) {
	static const auto original = hooks::m_model_render->get_original<hooks::draw_model_execute::T>(hooks::draw_model_execute::index);

	const auto player = reinterpret_cast<c_cs_player*>(interfaces::entity_list->get_client_entity(info.m_index));
	if (!player
		|| !player->is_enemy()
		|| !player->is_alive())
		return true;

	auto renderable = reinterpret_cast<c_base_entity*>(reinterpret_cast<DWORD>(info.m_renderable) - 0x4);
	if (!renderable)
		return true;

	override_material(M_REGULAR, col_t::palette_t::purple(), true);

	original(ecx, context, state, info, bones);

	return true;
}

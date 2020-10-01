#include "../utils.h"
#include "../../sdk/interfaces.h"

namespace netvars {
	void init() {
		for (auto data = interfaces::client_dll->get_all_classes(); data; data = data->m_next) {
			if (!data->m_recv_table)
				continue;

			dump_recursive(data->m_network_name, data->m_recv_table, 0);
		}
	}

	void dump_recursive(const char* base_class, c_recv_table* table, uint32_t offset) {
		for (auto i = 0; i < table->m_props_count; i++) {
			const auto prop = &table->m_props[i];
			if (!prop
				|| isdigit(prop->m_var_name[0])
				|| !strcmp(prop->m_var_name, _("baseclass")))
				continue;

			if (prop->m_data_table
				&& prop->m_recv_type == DPT_DATATABLE
				&& prop->m_data_table->m_net_table_name[0] == 'D') {
				dump_recursive(base_class, prop->m_data_table, offset + prop->m_offset);
			}

			char name[256];

			strcpy_s(name, base_class);
			strcat_s(name, _("->"));
			strcat_s(name, prop->m_var_name);

			m_list[FNV1A_RT(name)] = std::make_pair(prop, static_cast<uint32_t>(offset + prop->m_offset));
		}
	}

	std::unordered_map<uint32_t, std::pair<c_recv_prop*, uint32_t>> m_list;
}
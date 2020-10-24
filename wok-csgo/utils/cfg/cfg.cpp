#include "../utils.h"

#define ADD_ITEM(type, name, def) m_items[HASH(name)] = { HASH(#type), std::make_any<type>(def) };
#define ADD_ITEM_FLOAT_VEC(name, ...) m_items[HASH(name)] = { HASH("std::vector<float>"), std::make_any<std::vector<float>>({ __VA_ARGS__ }) };
#define ADD_ITEM_INT_VEC(name, ...) m_items[HASH(name)] = { HASH("std::vector<int>"), std::make_any<std::vector<int>>({ __VA_ARGS__ }) };
#define ADD_ITEM_BOOL_VEC(name, ...) m_items[HASH(name)] = { HASH("std::vector<bool>"), std::make_any<std::vector<bool>>({ __VA_ARGS__ }) };

namespace cfg {
	void init() {
		ADD_ITEM_FLOAT_VEC("example::clr3", 1.f, 1.f, 1.f)
		ADD_ITEM_FLOAT_VEC("example::clr4", 1.f, 1.f, 1.f, 1.f)
	}

	void save() {
		const auto path = std::filesystem::path(m_folder) /= m_name;

		std::filesystem::create_directory(path.parent_path());

		nlohmann::json out;

		for (auto& item : m_items) {
			nlohmann::json cur;

			cur[_("identifier")] = item.first;
			cur[_("type")] = item.second.m_type;

			switch (item.second.m_type) {
			case HASH("bool"): cur[_("inner")] = item.second.get<bool>(); break;
			case HASH("float"): cur[_("inner")] = item.second.get<float>(); break;
			case HASH("int"): cur[_("inner")] = item.second.get<int>(); break;
			case HASH("col_t"): {
				const auto clr = item.second.get<col_t>();

				nlohmann::json sub;

				for (auto& value : clr.m_value) {
					sub.push_back(value);
				}

				cur[_("inner")] = sub.dump();
			} break;
			case HASH("std::vector<int>"): {
				const auto vec = item.second.get<std::vector<int>>();

				nlohmann::json sub;

				for (auto& value : vec) {
					sub.push_back(value);
				}

				cur[_("inner")] = sub.dump();
			} break;
			case HASH("std::vector<float>"): {
				const auto vec = item.second.get<std::vector<float>>();

				nlohmann::json sub;

				for (auto& value : vec) {
					sub.push_back(value);
				}

				cur[_("inner")] = sub.dump();
			} break;
			case HASH("std::vector<bool>"): {
				const auto vec = item.second.get<std::vector<bool>>();

				nlohmann::json sub;

				for (const auto& value : vec) {
					sub.push_back(static_cast<int>(value));
				}

				cur[_("inner")] = sub.dump();
			} break;
			}

			out.push_back(cur);
		}

		std::ofstream file(path, std::ios::out | std::ios::trunc);

		file << out.dump(4);

		file.close();
	}

	void load() {
		const auto path = std::filesystem::path(m_folder) /= m_name;

		std::filesystem::create_directory(path.parent_path());

		nlohmann::json in;

		std::ifstream file(path, std::ios::in);

		file >> in;

		file.close();

		for (auto& item : in) {
			const auto hash = item[_("identifier")].get<uint32_t>();
			if (m_items.find(hash) == m_items.end())
				continue;

			auto& cur_item = m_items.at(hash);

			switch (item["type"].get<uint32_t>()) {
			case HASH("bool"): cur_item.set<bool>(item[_("inner")].get<bool>()); break;
			case HASH("float"): cur_item.set<float>(item[_("inner")].get<float>()); break;
			case HASH("int"): cur_item.set<int>(item[_("inner")].get<int>()); break;
			case HASH("col_t"): {
				const auto vec = nlohmann::json::parse(item[_("inner")].get<std::string>());

				cur_item.set<col_t>(col_t(vec.at(0u).get<uint8_t>(), vec.at(1u).get<uint8_t>(), vec.at(2u).get<uint8_t>(), vec.at(3u).get<uint8_t>()));
			} break;
			case HASH("std::vector<int>"): {
				const auto vec = nlohmann::json::parse(item[_("inner")].get<std::string>());

				auto& item_vec = cur_item.get<std::vector<int>>();

				for (auto i = 0u; i < vec.size(); i++) {
					if (i >= item_vec.size())
						continue;

					item_vec.at(i) = vec.at(i).get<int>();
				}
			} break;
			case HASH("std::vector<float>"): {
				const auto vec = nlohmann::json::parse(item[_("inner")].get<std::string>());

				auto& item_vec = cur_item.get<std::vector<float>>();

				for (auto i = 0u; i < vec.size(); i++) {
					if (i >= item_vec.size())
						continue;

					item_vec.at(i) = vec.at(i).get<float>();
				}
			} break;
			case HASH("std::vector<bool>"): {
				const auto vec = nlohmann::json::parse(item[_("inner")].get<std::string>());

				auto& item_vec = cur_item.get<std::vector<bool>>();

				for (auto i = 0u; i < vec.size(); i++) {
					if (i >= item_vec.size())
						continue;

					item_vec.at(i) = vec.at(i).get<int>();
				}
			} break;
			}
		}
	}

	std::string m_name = _("default.cfg");
	std::string m_folder = _("wok sdk v2");

	std::unordered_map<uint32_t, item_t> m_items;
}

#include "../utils.h"

#define ADD_ITEM(type, name, def) m_items[FNV1A(name)] = { FNV1A(#type), std::make_any<type>(def) };

namespace cfg {
	void init() {
		ADD_ITEM(bool, "example::bool", false)
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
			case FNV1A("bool"): cur[_("inner")] = item.second.get<bool>(); break;
			case FNV1A("float"): cur[_("inner")] = item.second.get<float>(); break;
			case FNV1A("int"): cur[_("inner")] = item.second.get<int>(); break;
			case FNV1A("col_t"): {
				const auto clr = item.second.get<col_t>();

				nlohmann::json sub;

				sub.push_back(clr.r());
				sub.push_back(clr.g());
				sub.push_back(clr.b());
				sub.push_back(clr.a());

				cur[_("inner")] = sub.dump();
			} break;
			case FNV1A("std::vector<int>"): {
				const auto vec = item.second.get<std::vector<int>>();

				nlohmann::json sub;

				for (auto& value : vec) {
					sub.push_back(value);
				}

				cur[_("inner")] = sub.dump();
			} break;
			case FNV1A("std::vector<float>"): {
				const auto vec = item.second.get<std::vector<float>>();

				nlohmann::json sub;

				for (auto& value : vec) {
					sub.push_back(value);
				}

				cur[_("inner")] = sub.dump();
			} break;
			case FNV1A("std::vector<bool>"): {
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
			case FNV1A("bool"): cur_item.set<bool>(item[_("inner")].get<bool>()); break;
			case FNV1A("float"): cur_item.set<float>(item[_("inner")].get<float>()); break;
			case FNV1A("int"): cur_item.set<int>(item[_("inner")].get<int>()); break;
			case FNV1A("col_t"): {
				const auto vec = nlohmann::json::parse(item[_("inner")].get<std::string>());

				cur_item.set<col_t>(col_t(vec.at(0).get<uint8_t>(), vec.at(1).get<uint8_t>(), vec.at(2).get<uint8_t>(), vec.at(3).get<uint8_t>()));
			} break;
			case FNV1A("std::vector<int>"): {
				const auto vec = nlohmann::json::parse(item[_("inner")].get<std::string>());

				auto& item_vec = cur_item.get<std::vector<int>>();

				for (auto i = 0u; i < vec.size(); i++) {
					if (i >= item_vec.size())
						continue;

					item_vec.at(i) = vec.at(i).get<int>();
				}
			} break;
			case FNV1A("std::vector<float>"): {
				const auto vec = nlohmann::json::parse(item[_("inner")].get<std::string>());

				auto& item_vec = cur_item.get<std::vector<float>>();

				for (auto i = 0u; i < vec.size(); i++) {
					if (i >= item_vec.size())
						continue;

					item_vec.at(i) = vec.at(i).get<float>();
				}
			} break;
			case FNV1A("std::vector<bool>"): {
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

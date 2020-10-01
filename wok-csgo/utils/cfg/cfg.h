#pragma once
#include "json/json.hpp"

namespace cfg {
	struct item_t {
		template <typename T>
		T& get() { return *reinterpret_cast<T*>(std::any_cast<T>(&m_var)); }

		template <typename T>
		void set(T val) { m_var.emplace<T>(val); }

		uint32_t m_type;

		std::any m_var;
	};

	void init();

	void save();

	void load();

	template <typename T>
	__declspec(noinline) static T& get(uint32_t hash) { return m_items.at(hash).get<T>(); }

	extern std::string m_name;
	extern std::string m_folder;

	extern std::unordered_map<uint32_t, item_t> m_items;
}
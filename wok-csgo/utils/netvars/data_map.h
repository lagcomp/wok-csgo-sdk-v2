#pragma once

namespace data_map {
	enum e_offset_type {
		OFFSET_TYPE_NORMAL,
		OFFSET_TYPE_PACKED
	};

	enum e_field_type {
		FIELDTYPE_VOID,
		FIELDTYPE_FLOAT,
		FIELDTYPE_STRING,
		FIELDTYPE_VEC3,
		FIELDTYPE_VEC4,
		FIELDTYPE_INT,
		FIELDTYPE_BOOL,
		FIELDTYPE_SHORT,
		FIELDTYPE_CHARACTER,
		FIELDTYPE_COLOR,
		FIELDTYPE_EMBEDDED,
		FIELDTYPE_CUSTOM,
		FIELDTYPE_CLASS_PTR,
		FIELDTYPE_ENTITY_HANDLE,
		FIELDTYPE_EDICT,
		FIELDTYPE_POSITION_VECTOR,
		FIELDTYPE_TIME,
		FIELDTYPE_TICK,
		FIELDTYPE_MODEL_NAME,
		FIELDTYPE_SOUND_NAME,
		FIELDTYPE_INPUT,
		FIELDTYPE_FUNCTION,
		FIELDTYPE_VIEW_MATRIX,
		FIELDTYPE_VIEW_MATRIX_WORLDSPACE,
		FIELDTYPE_MATRIX3X4_WORLDSPACE,
		FIELDTYPE_INTERVAL,
		FIELDTYPE_MODEL_INDEX,
		FIELDTYPE_MATERIAL_INDEX,
		FIELDTYPE_VEC2
	};

	struct data_map_t;
	struct input_data_t;

	using input_fn = void(__cdecl*)(input_data_t&);

	struct type_description_t {
		e_field_type			m_type;
		const char*				m_name;
		int						m_offset;
		uint16_t					m_size;
		uint16_t					m_flags;
		const char*				m_external_name;
		uintptr_t*				m_save_restore_ops;
		input_fn					m_input_fn;
		data_map_t*				m_data_map;
		uint32_t					m_size_in_bytes;
		type_description_t*	m_override;
		uint32_t					m_override_count;
		float						m_tolerance;
		int						m_flat_offset[2];
		uint16_t					m_flat_group;
	};

	struct data_map_t {
		type_description_t*	m_description;
		uint32_t					m_fields_count;
		const char*				m_data_class_name;
		data_map_t*				m_base_map;

		bool						m_chains_validated;
		bool						m_packed_offsets_computed;
		int						m_packed_size;
	};

	__declspec(noinline) static uint32_t get(data_map_t* map, uint32_t hash) {
		while (map) {
			for (auto i = 0u; i < map->m_fields_count; i++) {
				if (!map->m_description[i].m_name)
					continue;

				if (hash == FNV1A_RT(map->m_description[i].m_name))
					return map->m_description[i].m_offset;

				if (map->m_description[i].m_type == FIELDTYPE_EMBEDDED) {
					if (map->m_description[i].m_data_map) {
						if (const auto offset = get(map->m_description[i].m_data_map, hash))
							return offset;
					}
				}
			}

			map = map->m_base_map;
		}

		return 0;
	}
}

#define DATA_MAP(func, type, name) \
	__forceinline type& func { \
		static const auto offset = data_map::get(get_pred_desc_map(), FNV1A(name)); \
		return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset ); \
	}
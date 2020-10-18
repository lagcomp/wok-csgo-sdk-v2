#pragma once

enum e_offset_type {
	TD_OFFSET_NORMAL,
	TD_OFFSET_PACKED
};

enum e_field_type {
	FIELD_VOID,
	FIELD_FLOAT,
	FIELD_STRING,
	FIELD_VECTOR,
	FIELD_QUATERNION,
	FIELD_INTEGER,
	FIELD_BOOLEAN,
	FIELD_SHORT,
	FIELD_CHARACTER,
	FIELD_COLOR32,
	FIELD_EMBEDDED,
	FIELD_CUSTOM,
	FIELD_CLASSPTR,
	FIELD_EHANDLE,
	FIELD_EDICT,
	FIELD_POSITION_VECTOR,
	FIELD_TIME,
	FIELD_TICK,
	FIELD_MODELNAME,
	FIELD_SOUNDNAME,
	FIELD_INPUT,
	FIELD_FUNCTION,
	FIELD_VMATRIX,
	FIELD_VMATRIX_WORLDSPACE,
	FIELD_MATRIX3X4_WORLDSPACE,
	FIELD_INTERVAL,
	FIELD_MODELINDEX,
	FIELD_MATERIALINDEX,
	FIELD_VECTOR2D
};

struct data_map_t;
struct input_data_t;

using input_fn_t = void(__cdecl*)(input_data_t&);

struct type_description_t {
	e_field_type		m_field_type;
	const char*			m_field_name;
	int					m_field_offset;
	uint16_t			m_field_size;
	uint16_t				m_flags; // Maybe use bitflag here?
	const char*			m_external_name;
	uintptr_t*			m_save_restore_ops;
	input_fn_t			m_input_fn;
	data_map_t*			m_data_map;
	uint32_t					m_field_size_in_bytes;
	type_description_t*	m_override_field;
	uint32_t					m_override_count;
	float				m_field_tolerance;
	int					m_flat_offset[2];
	uint16_t			m_flat_group;
};

struct data_map_t {
	__forceinline uint32_t find(uint32_t hash) const {
		auto data_map = this;

		while (data_map) {
			for (auto i = 0u; i < data_map->m_data_fields_count; i++) {
				if (!data_map->m_data_description[i].m_field_name)
					continue;

				if (hash == FNV1A_RT(data_map->m_data_description[i].m_field_name))
					return data_map->m_data_description[i].m_field_offset;

				if (data_map->m_data_description[i].m_field_type != FIELD_EMBEDDED
					|| !data_map->m_data_description[i].m_data_map)
					continue;

				const auto offset = data_map->m_data_description[i].m_data_map->find(hash);
				if (!offset)
					continue;

				return offset;
			}

			data_map = data_map->m_base_map;
		}

		return 0u;
	}

	type_description_t*	m_data_description;
	int                 m_data_fields_count;
	char const*			m_data_class_name;
	data_map_t*			m_base_map;

	bool                m_chains_validated;
	bool                m_packed_offsets_computed;
	int                 m_packed_size;
};

#define DATA_MAP(func, type, name) \
	__forceinline type& func { \
		static const auto offset = get_pred_desc_map()->find(FNV1A(name)); \
		return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset ); \
	}

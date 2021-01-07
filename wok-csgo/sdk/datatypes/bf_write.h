#pragma once

class c_bf_write {
public:
	c_bf_write() {
		m_data = nullptr;
		m_data_bytes = 0;
		m_data_bits = -1;
		m_cur_bit = 0;
		m_overflow = false;
		m_assert_on_overflow = true;
		m_debug_name = "";
	}

	c_bf_write(void* data, int bytes, int bits = -1) {
		m_assert_on_overflow = true;
		m_debug_name = "";

		start_writing(data, bytes, 0, bits);
	}

	c_bf_write(const char* debug_name, void* data, int bytes, int bits = -1) {
		m_assert_on_overflow = true;
		m_debug_name = debug_name;

		start_writing(data, bytes, 0, bits);
	}

	__forceinline void start_writing(void* data, int bytes, int start_bit = 0, int bits = -1) {
		bytes &= ~3;

		m_data = reinterpret_cast<uint8_t*>(data);
		m_data_bytes = bytes;
		m_data_bits = bits == -1 ? bytes << 3 : bits;

		m_cur_bit = start_bit;
		m_overflow = false;
	}

	__forceinline uint32_t get_bytes_written_count() const { return (m_cur_bit + 7) >> 3; }

	__forceinline void write_user_cmd(void* to, void* from) {
		static const auto write_user_cmd_fn = SIG("client.dll", "55 8B EC 83 E4 F8 51 53 56 8B D9 8B 0D");

		__asm {
			mov     ecx, this
			mov     edx, to
			push	from
			call    write_user_cmd_fn
			add     esp, 4
		}
	}

	uint8_t*		m_data;
	int				m_data_bytes;
	int				m_data_bits;
	int				m_cur_bit;
	bool			m_overflow;
	bool			m_assert_on_overflow;
	const char*		m_debug_name;
};

#pragma once

class bf_write {
public:
	unsigned char*	m_data;
	int				m_data_bytes;
	int				m_data_bits;
	int				m_cur_bit;
	bool			m_overflow;
	bool			m_assert_on_overflow;
	const char*		m_debug_name;

	void start_writing(void* data, int bytes, int start_bit = 0, int bits = -1) {
		bytes &= ~3;

		m_data = (unsigned char*)data;
		m_data_bytes = bytes;

		m_data_bits = (bits == -1) ? bytes << 3 : bits;

		m_cur_bit = start_bit;
		m_overflow = false;
	}

	bf_write() {
		m_data = 0;
		m_data_bytes = 0;
		m_data_bits = -1;
		m_cur_bit = 0;
		m_overflow = false;
		m_assert_on_overflow = true;
		m_debug_name = 0;
	}

	bf_write(void* data, int bytes, int bits = -1) {
		m_assert_on_overflow = true;
		m_debug_name = 0;
		start_writing(data, bytes, 0, bits);
	}

	bf_write(const char* debug_name, void* data, int bytes, int bits = -1) {
		m_assert_on_overflow = true;
		m_debug_name = debug_name;
		start_writing(data, bytes, 0, bits);
	}
};

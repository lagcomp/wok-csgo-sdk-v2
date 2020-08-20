#pragma once

class bf_read {
public:
	const char*				m_debug_name;
	bool					m_overflow;
	int						m_data_bits;
	unsigned int			m_data_bytes;
	unsigned int			m_in_buf_word;
	int						m_bits_avail;
	const unsigned int*		m_data_in;
	const unsigned int*		m_buffer_end;
	const unsigned int*		m_data;

	bf_read() = default;

	bf_read(const void* data, int bytes, int bits = -1) {
		start_reading(data, bytes, 0, bits);
	}

	void start_reading(const void* data, int bytes, int start_bit, int bits) {
		m_data = (uint32_t*)data;
		m_data_in = m_data;
		m_data_bytes = bytes;

		m_data_bits = (bits == -1) ? (bytes << 3) : bits;

		m_overflow = false;
		m_buffer_end = reinterpret_cast<uint32_t const*>(reinterpret_cast<uint8_t const*>(m_data) + bytes);

		if (m_data)
			seek(start_bit);
	}

	bool seek(int pos) {
		bool succ = true;
		if (pos < 0
			|| pos > m_data_bits) {
			m_overflow = true;
			succ = false;
			pos = m_data_bits;
		}

		int head = m_data_bytes & 3;
		int byeteofs = pos / 8;

		if ((m_data_bytes < 4) 
			|| (head && (byeteofs < head))) {
			uint8_t const* partial = (uint8_t const*)m_data;
			if (m_data) {
				m_in_buf_word = *(partial++);
				if (head > 1)
					m_in_buf_word |= (*partial++) << 8;

				if (head > 2)
					m_in_buf_word |= (*partial++) << 16;
			}

			m_data_in = (uint32_t const*)partial;
			m_in_buf_word >>= (pos & 31);
			m_bits_avail = (head << 3) - (pos & 31);
		}
		else {
			int adj_pos = pos - (head << 3);
			m_data_in = reinterpret_cast<uint32_t const*>(
				reinterpret_cast<uint8_t const*>(m_data) + ((adj_pos / 32) << 2) + head);

			if (m_data) {
				m_bits_avail = 32;
				grab_next_dword();
			}
			else {
				m_in_buf_word = 0;
				m_bits_avail = 1;
			}

			m_in_buf_word >>= (adj_pos & 31);
			m_bits_avail = min(m_bits_avail, 32 - (adj_pos & 31));
		}

		return succ;
	}

	__forceinline void grab_next_dword(bool overflow_immediately = false) {
		if (m_data_in == m_buffer_end) {
			m_bits_avail = 1;
			m_in_buf_word = 0;
			m_data_in++;

			if (overflow_immediately)
				m_overflow = true;
		}
		else if (m_data_in > m_buffer_end) {
			m_overflow = true;
			m_in_buf_word = 0;
		}
		else {
			m_in_buf_word = DWORD(*(m_data_in++));
		}
	}
};

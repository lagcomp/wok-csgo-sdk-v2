#pragma once

class c_bf_read {
public:
	const char*			m_debug_name;
	bool				m_overflow;
	int					m_data_bits;
	uint32_t			m_data_bytes;
	uint32_t			m_in_buf_word;
	int					m_bits_avail;
	const uint32_t*		m_data_in;
	const uint32_t*		m_buffer_end;
	const uint32_t*		m_data;

	c_bf_read() = default;

	c_bf_read(const void* data, int bytes, int bits = -1) { start_reading(data, bytes, 0, bits); }

	__forceinline void start_reading(const void* data, int bytes, int start_bit, int bits) {
		m_data = reinterpret_cast<uint32_t const*>(data);
		m_data_in = m_data;
		m_data_bytes = bytes;

		m_data_bits = bits == -1 ? bytes << 3 : bits;

		m_overflow = false;
		m_buffer_end = reinterpret_cast<uint32_t const*>(reinterpret_cast<uint8_t const*>(m_data) + bytes);

		if (m_data) {
			seek(start_bit);
		}
	}

	__forceinline bool seek(int pos) {
		auto succ = true;
		if (pos < 0
			|| pos > m_data_bits) {
			m_overflow = true;
			succ = false;
			pos = m_data_bits;
		}

		const auto head = m_data_bytes & 3;
		const auto byeteofs = pos / 8;

		if (m_data_bytes < 4
			|| head && byeteofs < head) {
			auto partial = reinterpret_cast<uint8_t const*>(m_data);

			if (m_data) {
				m_in_buf_word = *(partial++);

				if (head > 1) {
					m_in_buf_word |= (*partial++) << 8;
				}

				if (head > 2) {
					m_in_buf_word |= (*partial++) << 16;
				}
			}

			m_data_in = reinterpret_cast<uint32_t const*>(partial);
			m_in_buf_word >>= (pos & 31);
			m_bits_avail = (head << 3) - (pos & 31);
		}
		else {
			const auto adj_pos = pos - (head << 3);

			m_data_in = reinterpret_cast<uint32_t const*>(reinterpret_cast<uint8_t const*>(m_data) + ((adj_pos / 32) << 2) + head);

			if (m_data) {
				m_bits_avail = 32;

				grab_next_dword();
			}
			else {
				m_in_buf_word = 0u;
				m_bits_avail = 1;
			}

			m_in_buf_word >>= (adj_pos & 31);
			m_bits_avail = std::min<int>(m_bits_avail, 32 - (adj_pos & 31));
		}

		return succ;
	}

	__forceinline void grab_next_dword(bool overflow_immediately = false) {
		if (m_data_in == m_buffer_end) {
			m_bits_avail = 1;
			m_in_buf_word = 0u;
			m_data_in++;

			if (overflow_immediately) {
				m_overflow = true;
			}
		}
		else if (m_data_in > m_buffer_end) {
			m_overflow = true;
			m_in_buf_word = 0u;
		}
		else {
			m_in_buf_word = *(m_data_in++);
		}
	}
};
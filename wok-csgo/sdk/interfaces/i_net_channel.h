#pragma once

class i_net_channel_info {
public:
	virtual const char* get_name() const = 0;
	virtual const char* get_address() const = 0;
	virtual float		get_time() const = 0;
	virtual float		get_time_connected() const = 0;
	virtual int			get_buffer_size() const = 0;
	virtual int			get_data_rate() const = 0;

	virtual bool		is_loopback() const = 0;
	virtual bool		is_timing_out() const = 0;
	virtual bool		is_playback() const = 0;

	virtual float		get_latency(int flow) const = 0;
	virtual float		get_avg_latency(int flow) const = 0;
	virtual float		get_avg_loss(int flow) const = 0;
	virtual float		get_avg_choke(int flow) const = 0;
	virtual float		get_avg_data(int flow) const = 0;
	virtual float		get_avg_packets(int flow) const = 0;
	virtual int			get_total_data(int flow) const = 0;
	virtual int			get_sequence_number(int flow) const = 0;
	virtual bool		is_valid_packet(int flow, int frame_number) const = 0;
	virtual float		get_packet_time(int flow, int frame_number) const = 0;
	virtual int			get_packet_bytes(int flow, int frame_number, int group) const = 0;
	virtual bool		get_stream_progress(int flow, int* received, int* total) const = 0;
	virtual float		get_time_since_last_received(void) const = 0;
	virtual	float		get_command_interpolation_amount(int flow, int frame_number) const = 0;
	virtual void		get_packet_response_latency(int flow, int frame_number, int* latency, int* choke) const = 0;
	virtual void		get_remote_framerate(float* frame_time, float* frame_time_std_deviation) const = 0;

	virtual float		get_timeout_seconds() const = 0;
};

class i_net_channel;

class i_net_msg {
public:
	virtual					~i_net_msg() = default;
	virtual void			set_net_channel(void* net_channel) = 0;
	virtual void			set_reliable(bool state) = 0;
	virtual bool			process() = 0;
	virtual	bool			read_from_buffer(void* buffer) = 0;
	virtual	bool			write_to_buffer(void* buffer) = 0;
	virtual bool			is_reliable() const = 0;
	virtual int				get_type() const = 0;
	virtual int				get_group() const = 0;
	virtual const char*		get_name() const = 0;
	virtual i_net_channel*		get_net_channel() const = 0;
	virtual const char*		to_string() const = 0;
};

template <typename T>
class i_net_msg_pb : public i_net_msg, public T {
public:

};

class i_move_msg {
public:
	char			pad0[8];
	int				m_backup_commands;
	int				m_new_commands;
	std::string*	m_data;
	bf_read			m_data_in;
	bf_write		m_data_out;
};

using i_move_msg_t = i_net_msg_pb<i_move_msg>;

class i_net_channel {
public:
	VFUNC(set_time_out(float time), 4, void(__thiscall*)(void*, float), time)
	VFUNC(get_latency(int flow), 9, float(__thiscall*)(void*, int), flow)
	VFUNC(send_net_msg(i_net_msg* msg, bool reliable, bool voice), 40, bool(__thiscall*)(void*, i_net_msg*, bool, bool), msg, reliable, voice)
	VFUNC(send_datagram(), 46, int(__thiscall*)(void*, void*), nullptr)
	VFUNC(transmit(bool reliable), 49, bool(__thiscall*)(void*, bool), reliable)
	VFUNC(request_file(const char* file_name), 62, int(__thiscall*)(void*, const char*), file_name)

	__forceinline static uintptr_t* get_vtable() {
		static const auto vtable = SIG("engine.dll", "C7 06 ? ? ? ? 8D BE ? ? ? ?").self_offset(0x2).cast<uintptr_t*>();

		return vtable;
	}

	char pad0[20];
	bool m_processing_messages;
	bool m_should_delete;
	char pad1[2];
	int	 m_out_sequence_number;
	int	 m_in_sequence_number;
	int  m_out_sequence_number_ack;
	int  m_out_reliable_state;
	int  m_in_reliable_state;
	int  m_choked_packets;
	char pad2[1044];
};

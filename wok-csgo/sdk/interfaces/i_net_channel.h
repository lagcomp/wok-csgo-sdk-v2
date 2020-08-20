#pragma once

constexpr auto FLOW_OUTGOING = 0;
constexpr auto FLOW_INCOMING = 1;
constexpr auto MAX_FLOWS = 2;

class i_net_msg {
public:
	virtual					~i_net_msg() = default;
	virtual void			set_net_channel(void* net_channel) = 0;
	virtual void			set_reliable(bool state) = 0;
	virtual bool			process() = 0;
	virtual	bool			read_from_buffer(void* buf) = 0;
	virtual	bool			write_to_buffer(void* buf) = 0;
	virtual bool			is_reliable() const = 0;
	virtual int				get_type() const = 0;
	virtual int				get_group() const = 0;
	virtual const char*		get_name() const = 0;
	virtual void*			get_net_channel() const = 0;
	virtual const char*		to_string() const = 0;
};

template<typename T>
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

typedef i_net_msg_pb<i_move_msg> i_move_msg_t;

class i_net_channel {
public:
	VFUNC(get_latency(const int flow), 9, float(__thiscall*)(i_net_channel*, int), flow)
	VFUNC(send_netmsg(i_net_msg* msg, bool reliable, bool voice), 40, bool(__thiscall*)(i_net_channel*, i_net_msg*, bool, bool), msg, reliable, voice)
	VFUNC(send_datagram(), 46, int(__thiscall*)(i_net_channel*, void*), nullptr)

	char pad0[20];
	bool m_processing_messages;
	bool m_should_delete;
	char pad1[2];
	int	 m_out_sequence_nr;
	int	 m_in_sequence_nr;
	int  m_out_sequence_nr_ack;
	int  m_out_reliable_state;
	int  m_in_reliable_state;
	int  m_choked_packets;
	char pad2[1044];
};

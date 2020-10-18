#pragma once

class i_game_event {
public:
	virtual					~i_game_event() = default;
	virtual const char*		get_name() const = 0;

	virtual bool			is_reliable() const = 0;
	virtual bool			is_local() const = 0;
	virtual bool			is_empty(const char* name = 0) = 0;

	virtual bool			get_bool(const char* name, bool def_value = false) = 0;
	virtual int				get_int(const char* name, int def_value = 0) = 0;
	virtual unsigned long	get_uint64(const char* name, unsigned long def_value = 0) = 0;
	virtual float			get_float(const char* name, float def_value = 0.f) = 0;
	virtual const char*		get_string(const char* name, const char* def_value = "") = 0;
	virtual const wchar_t*	get_wstring(const char* name, const wchar_t* def_value = L"") = 0;
	virtual const void*		get_ptr(const char* name, const void* def_value = nullptr) = 0;

	virtual void			set_ptr(const char* name, const void* value) = 0;
	virtual void			set_bool(const char* name, bool value) = 0;
	virtual void			set_int(const char* name, int value) = 0;
	virtual void			set_uint64(const char* name, unsigned long value) = 0;
	virtual void			set_float(const char* name, float value) = 0;
	virtual void			set_string(const char* name, const char* value) = 0;
	virtual void			set_wstring(const char* name, const wchar_t* value) = 0;
};

class i_game_event_listener {
public:
	virtual			~i_game_event_listener() = default;
	virtual void	fire_game_event(i_game_event* event) = 0;
	virtual int		get_event_debug_id() { return EVENT_DEBUG_ID_INIT; }
};

class i_game_event_manager {
public:
	virtual					~i_game_event_manager() = default;
	virtual int				load_events_from_file(const char* name) = 0;
	virtual void			reset() = 0;
	virtual bool			add_listener(i_game_event_listener* listener, const char* name, bool server_side) = 0;
	virtual bool			find_listener(i_game_event_listener* listener, const char* name) = 0;
	virtual void			remove_listener(i_game_event_listener* listener) = 0;
	virtual void			add_listener_global(i_game_event_listener* listener, bool server_side) = 0;
	virtual i_game_event*	create_event(const char* name, bool force = false, int* cookie = 0) = 0;
	virtual bool			fire_event(i_game_event* event, bool dont_broadcast = false) = 0;
	virtual bool			fire_event_client_side(i_game_event* event) = 0;
	virtual i_game_event*	duplicate_event(i_game_event* event) = 0;
	virtual void			free_event(i_game_event* event) = 0;
	virtual bool			serialize_event(i_game_event* event, bf_write* buf) = 0;
	virtual i_game_event*	unserialize_event(bf_read* buf) = 0;
	virtual void*			get_event_data_types(i_game_event* event) = 0;
};

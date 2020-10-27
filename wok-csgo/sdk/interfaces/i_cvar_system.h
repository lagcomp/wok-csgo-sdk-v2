#pragma once

using create_interface_fn = uintptr_t*(__cdecl*)(const char* name, int* code);

class i_app_system {
public:
	virtual bool            connect(create_interface_fn factory) = 0;
	virtual void            disconnect() = 0;
	virtual void*			query_interface(const char* name) = 0;
	virtual int				init() = 0;
	virtual void            shutdown() = 0;
	virtual const void*		get_dependencies() = 0;
	virtual int				get_tier() = 0;
	virtual void            reconnect(create_interface_fn factory, const char* name) = 0;
	virtual void			pad0() = 0;
	virtual void			pad1() = 0;
};

class i_cvar_system : public i_app_system {
public:
	virtual int				allocate_dll_identifier() = 0;
	virtual void			register_cmd(c_cvar* cvar) = 0;
	virtual void			unregister_cmd(c_cvar* cvar) = 0;
	virtual void			unregister_cmds(int id) = 0;
	virtual c_cvar*			find_var(const char* name) = 0;

	template <typename... A>
	VFUNC(console_print(const col_t& clr, const char* txt, A... args), 25, void(__cdecl*)(void*, const col_t&, const char*, ...), clr, txt, args...)
};

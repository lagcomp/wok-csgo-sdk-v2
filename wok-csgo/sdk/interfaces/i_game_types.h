#pragma once

class i_game_types
{
public:
	VFUNC(get_current_game_type(), 8, int(__thiscall*)(void*))
	VFUNC(get_current_game_mode(), 9, int(__thiscall*)(void*))
	VFUNC(get_current_map_name(), 10, const char*(__thiscall*)(void*))
	VFUNC(get_current_game_type_name_id(), 11, const char*(__thiscall*)(void*))
	VFUNC(get_current_game_mode_name_id(), 13, const char*(__thiscall*)(void*))
};
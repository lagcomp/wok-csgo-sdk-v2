#pragma once

class i_game_types {
public:
	VFUNC(get_cur_game_type(), 8, int(__thiscall*)(void*))
	VFUNC(get_cur_game_mode(), 9, int(__thiscall*)(void*))
	VFUNC(get_cur_map_name(), 10, const char*(__thiscall*)(void*))
	VFUNC(get_cur_game_type_name_id(), 11, const char*(__thiscall*)(void*))
	VFUNC(get_cur_game_mode_name_id(), 12, const char*(__thiscall*)(void*))
};

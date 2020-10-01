#pragma once
#include "../../globals.h"

class c_movement : public c_singleton<c_movement> {
public:
	void fix(const angle_t& wish, const angle_t& original);
};
#define movement c_movement::instance()
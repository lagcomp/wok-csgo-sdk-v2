#pragma once
#include "../../globals.h"

class c_movement : public c_singleton<c_movement> {
public:
	void fix(const qangle_t& wish, const qangle_t& original);
	void bhop();
};
#define movement c_movement::instance()

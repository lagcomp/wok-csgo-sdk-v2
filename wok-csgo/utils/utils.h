#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS

#include "../common_includes.h"
#include "../sdk/definitions.h"

#include "math/datatypes.h"
#include "math/math.h"

#include "obfuscation/fnv1a.h"
#include "obfuscation/xor.h"

#include "memory/memory.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_freetype.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

#include "render/render.h"

#include "netvars/data_map.h"
#include "netvars/netvars.h"

#include "input/input.h"

#include "cfg/cfg.h"

namespace utils {
	__forceinline std::string to_utf8(const std::wstring& txt) {
		if (txt.empty())
			return "";

		return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(txt);
	}

	__forceinline std::wstring to_unicode(const std::string& txt) {
		if (txt.empty())
			return L"";

		return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(txt);
	}
}

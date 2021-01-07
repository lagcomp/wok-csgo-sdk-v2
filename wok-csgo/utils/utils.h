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
	__forceinline std::string to_utf8(std::wstring_view txt) {
		if (txt.empty())
			return "";

		const auto size = WideCharToMultiByte(CP_UTF8, 0, txt.data(), txt.size(), 0, 0, 0, 0);

		auto ret = std::string(size, 0);

		WideCharToMultiByte(CP_UTF8, 0, txt.data(), txt.size(), ret.data(), size, 0, 0);

		return ret;
	}

	__forceinline std::wstring to_unicode(std::string_view txt) {
		if (txt.empty())
			return L"";

		const auto size = MultiByteToWideChar(CP_UTF8, 0, txt.data(), txt.size(), 0, 0);

		auto ret = std::wstring(size, 0);

		MultiByteToWideChar(CP_UTF8, 0, txt.data(), txt.size(), ret.data(), size);

		return ret;
	}
}

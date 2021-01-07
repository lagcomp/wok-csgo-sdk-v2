#pragma once
#define NOMINMAX
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include <Windows.h>
#include <stdint.h>
#include <fstream>
#include <chrono>
#include <optional>
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <thread>
#include <memory>
#include <cstdint>
#include <functional>
#include <mutex>
#include <unordered_map>
#include <TlHelp32.h>
#include <cmath>
#include <algorithm>
#include <array>
#include <windowsx.h>
#include <intrin.h>
#include <any>
#include <filesystem>
#include <winternl.h>
#include <locale>
#include <codecvt>

#include <d3d9.h>
#include <d3dx9core.h>

#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "freetype.lib")

#pragma intrinsic(_AdressOfReturnAddress)
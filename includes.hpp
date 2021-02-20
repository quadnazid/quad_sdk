#include <windows.h>
#include <iostream>
#include <cstdint>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <array>
#include <fstream>
#include <istream>
#include <unordered_map>
#include <intrin.h>

#include "hack/globals/sdk.hpp"
#include "hack/interfaces/interfaces.hpp"
#include "hack/hooks/hooks.hpp"
#include "hack/hooks/minhooks/minhook.h"
#include "hack/drawing/drawing.hpp"
#include "hack/globals/math/math.hpp"

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


// defines
namespace std {
#define _int32 int32_t
}
#define main DllMain
#define module HMODULE
#define disable_calls DisableThreadLibraryCalls

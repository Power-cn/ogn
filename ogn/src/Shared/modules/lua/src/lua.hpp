// lua.hpp
// Lua header files for C++
// <<extern "C">> not supplied automatically because Lua also compiles as C++

//#define LUA_COMPAT_5_1
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#pragma once
#include "Shared.hpp"
#include "ConfigManager.h"
#include "Session.h"
#include "Entity.h"
#include "Player.h"
#include "SessionManager.h"
#include "protocols/protocols.h"
#include "Application.h"


#ifdef WIN32
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libevent/libevent.lib")
#pragma comment(lib, "libevent/libevent_core.lib")
#pragma comment(lib, "libevent/libevent_extras.lib")

#ifdef _DEBUG

#pragma comment(lib, "Shared_d.lib")

#else

#pragma comment(lib, "Shared.lib")

#endif // DEBUG

#endif

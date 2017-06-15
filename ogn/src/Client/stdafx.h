#pragma once

#include "Shared.h"
#include "protocols/protocols.h"
#include "Property.h"
#include <windows.h>
#include "ConfigManager.h"
#include "Robot.h"
#include "RobotManager.h"
#include "SocketHandler.h"
#include "Utils.h"


#ifdef WIN32
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libevent.lib")
#pragma comment(lib, "libevent_core.lib")
#pragma comment(lib, "libevent_extras.lib")

#ifdef _DEBUG

#pragma comment(lib, "Shared_d.lib")

#else

#pragma comment(lib, "Shared.lib")

#endif // DEBUG

#endif
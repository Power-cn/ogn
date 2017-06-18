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

#ifdef _DEBUG

#pragma comment(lib, "Shared_d.lib")

#else

#pragma comment(lib, "Shared.lib")

#endif // DEBUG

#endif

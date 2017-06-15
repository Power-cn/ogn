#include "Shared.hpp"

#include "ConfigManager.h"
#include "Entity.h"
#include "Player.h"
#include "Session.h"
#include "SessionManager.h"
#include "protocols/protocols.h"
#include "Application.h"

#include "mysql.hpp"
#include "DBTable.h"
#include "SessionHandler.h"


#ifdef WIN32

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libevent.lib")
#pragma comment(lib, "libevent_core.lib")
#pragma comment(lib, "libevent_extras.lib")
#pragma comment(lib, "libmysql.lib")

#ifdef _DEBUG

#pragma comment(lib, "mysql_d.lib")
#pragma comment(lib, "Shared_d.lib")

#else

#pragma comment(lib, "mysql.lib")
#pragma comment(lib, "Shared.lib")

#endif // DEBUG

#endif

#pragma once

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



#ifndef LINUX


#ifdef WIN64

	#ifdef _DEBUG

		#pragma comment(lib, "libmysql/libmysqld.lib")
		#pragma comment(lib, "mysql64_d.lib")
		#pragma comment(lib, "Shared64_d.lib")

	#else
		#pragma comment(lib, "libmysql/libmysql.lib")

		#pragma comment(lib, "mysql64.lib")
		#pragma comment(lib, "Shared64.lib")

	#endif // DEBUG

#else

	#ifdef _DEBUG

	#pragma comment(lib, "mysql_d.lib")
	#pragma comment(lib, "Shared_d.lib")

	#else

	#pragma comment(lib, "mysql.lib")
	#pragma comment(lib, "Shared.lib")

	#endif // DEBUG

#endif

#endif // !LINUX
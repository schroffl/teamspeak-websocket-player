#include <thread>
#include "wsserver.h"

#ifdef _WIN32
#include <Windows.h>
typedef HANDLE GlobalThread;
#else
typedef std::thread GlobalThread;
#endif

class Globals {
	public:
		static GlobalThread mainThread;
		static std::unique_ptr<WebSocketServer> wss;
};
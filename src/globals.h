#include <thread>

#ifdef _WIN32
#include <Windows.h>
typedef HANDLE GlobalThread;
#else
typdef std::thread GlobalThread;
#endif

class Globals {
	public:
		static GlobalThread mainThread;
};
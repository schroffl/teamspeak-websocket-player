#include "globals.h"

GlobalThread Globals::mainThread;
std::unique_ptr<WebSocketServer> Globals::wss;
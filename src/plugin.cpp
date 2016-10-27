/*
 * TeamSpeak 3 demo plugin
 *
 * Copyright (c) 2008-2016 TeamSpeak Systems GmbH
 */

#ifdef _WIN32
#pragma warning (disable : 4100)  /* Disable Unreferenced parameter warning */
#else
#include <thread>
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include "teamspeak/public_errors.h"
#include "teamspeak/public_errors_rare.h"
#include "teamspeak/public_definitions.h"
#include "teamspeak/public_rare_definitions.h"
#include "teamspeak/clientlib_publicdefinitions.h"
#include "ts3_functions.h"
#include "plugin.h"

static struct TS3Functions ts3Functions;

#include "globals.h"
#include "config.h"
#include "wsserver.h"

#define PLUGIN_NAME "WebSocket Stream Player"
#define PLUGIN_API_VERSION 20

#define DEFAULT_PCM_BUFFER_SIZE 16384
#define DEFAULT_WEBSOCKET_SERVER_PORT 8080

#ifdef _WIN32
DWORD WINAPI MainThread(LPVOID lpParam);
#else
void MainThread();
#endif

Config cfg;

WebSocketServer wss;
int websocketServerPort;

/*********************************** Required functions ************************************/
/*
 * If any of these required functions is not implemented, TS3 will refuse to load the plugin
 */

/* Unique name identifying this plugin */
const char* ts3plugin_name() {
	return PLUGIN_NAME;
}

/* Plugin version */
const char* ts3plugin_version() {
    return "0.2.0";
}

/* Plugin API version. Must be the same as the clients API major version, else the plugin fails to load. */
int ts3plugin_apiVersion() {
	return PLUGIN_API_VERSION;
}

/* Plugin author */
const char* ts3plugin_author() {
    return "schroffl";
}

/* Plugin description */
const char* ts3plugin_description() {
	/* If you want to use wchar_t, see ts3plugin_name() on how to use */
    return "Pipes the audio from a WebSocket into an input device";
}

/* Set TeamSpeak 3 callback functions */
void ts3plugin_setFunctionPointers(const struct TS3Functions funcs) {
    ts3Functions = funcs;
}

int ts3plugin_init() {
	printf("[%s] Init\n", PLUGIN_NAME);

	char pluginPath[512];
	char configPath[512];

	// Acquire the path of the configuration file
	ts3Functions.getPluginPath(pluginPath, 512);
	sprintf(configPath, "%sws-replay/config.ini", pluginPath);

	cfg.loadFile(configPath);
	websocketServerPort = cfg.get("port", DEFAULT_WEBSOCKET_SERVER_PORT);

	const unsigned int err = ts3Functions.registerCustomDevice("wsstream", "WebSocket Stream", 48000, 1, 48000, 1);

	if (err != ERROR_ok)
		printf("[%s] Failed to register device [ERROR: %i]\n", PLUGIN_NAME, err);

	#ifdef _WIN32
	Globals::mainThread = CreateThread(NULL, 0, MainThread, NULL, 0, NULL);
	#else
	Globals::mainThread = std::thread(MainThread);
	#endif

	return 0;
}

void ts3plugin_shutdown() {
	const unsigned int err = ts3Functions.unregisterCustomDevice("wssstream");

	if (err != ERROR_ok)
		printf("[%s] Failed to unregister device [ERROR: %i]\n", PLUGIN_NAME, err);

	wss.stop();

	#ifdef _WIN32
	WaitForSingleObject(Globals::mainThread, INFINITE);
	#else
	Globals::mainThread.join();
	#endif

	printf("[%s] Shutdown\n", PLUGIN_NAME);
}

void on_message(websocketpp::connection_hdl hdl, message_ptr msg) {
	std::string msgString = msg->get_payload();

	char charBuf[DEFAULT_PCM_BUFFER_SIZE * 2];
	short pcmBuf[DEFAULT_PCM_BUFFER_SIZE];

	memcpy(charBuf, msgString.c_str(), msgString.size());

	for (int i = 0; i < DEFAULT_PCM_BUFFER_SIZE * 2; i += 2) {
		int sample = i / 2;

		pcmBuf[sample] = ((charBuf[i + 1] & 0xFF) << 8) | (charBuf[i] & 0xFF);
	}

	ts3Functions.processCustomCaptureData("wsstream", pcmBuf, DEFAULT_PCM_BUFFER_SIZE);
}

#ifdef _WIN32
DWORD WINAPI MainThread(LPVOID lpParam) {
	wss.run(websocketServerPort);
	return 0;
}
#else
void MainThread() {
	wss.run(websocketServerPort);
}
#endif
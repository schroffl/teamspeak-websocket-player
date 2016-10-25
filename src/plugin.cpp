/*
 * TeamSpeak 3 demo plugin
 *
 * Copyright (c) 2008-2016 TeamSpeak Systems GmbH
 */

#ifdef _WIN32
	#pragma warning (disable : 4100)  /* Disable Unreferenced parameter warning */
	#include <Windows.h>
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "teamspeak/public_errors.h"
#include "teamspeak/public_errors_rare.h"
#include "teamspeak/public_definitions.h"
#include "teamspeak/public_rare_definitions.h"
#include "teamspeak/clientlib_publicdefinitions.h"
#include "ts3_functions.h"
#include "plugin.h"

#include "wsserver.h"

static struct TS3Functions ts3Functions;

#ifdef _WIN32
#define _strcpy(dest, destSize, src) strcpy_s(dest, destSize, src)
#define snprintf sprintf_s
#else
#define _strcpy(dest, destSize, src) { strncpy(dest, src, destSize-1); (dest)[destSize-1] = '\0'; }
#endif

#define PLUGIN_API_VERSION 20

#define PCM_BUFFER_SIZE 16384
#define WEBSOCKET_SERVER_PORT 8080

DWORD WINAPI MainThread(LPVOID lpParam);

WebSocketServer *wss;

/*********************************** Required functions ************************************/
/*
 * If any of these required functions is not implemented, TS3 will refuse to load the plugin
 */

/* Unique name identifying this plugin */
const char* ts3plugin_name() {
#ifdef _WIN32
	return "WebSocket Stream";
#else
	return "WebSocket Stream";
#endif
}

/* Plugin version */
const char* ts3plugin_version() {
    return "0.1.0";
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
    return "Play the audio from a websocket stream";
}

/* Set TeamSpeak 3 callback functions */
void ts3plugin_setFunctionPointers(const struct TS3Functions funcs) {
    ts3Functions = funcs;
}

int ts3plugin_init() {
	printf("init\n");

	WebSocketServer inst;
	wss = &inst;

	CreateThread(NULL, 0, MainThread, NULL, 0, NULL);

	return 0;
}

void ts3plugin_shutdown() {
	if (ts3Functions.unregisterCustomDevice("wsstream") != ERROR_ok)
		printf("Failed to unregister device\n");

	wss->stop();

	printf("shutdown\n");
}

void on_message(server *s, websocketpp::connection_hdl hdl, message_ptr msg) {
	std::string msgString = msg->get_payload();

	char charBuf[PCM_BUFFER_SIZE * 2];
	short pcmBuf[PCM_BUFFER_SIZE];

	memcpy(charBuf, msgString.c_str(), msgString.size());

	for (int i = 0; i < PCM_BUFFER_SIZE * 2; i += 2) {
		int sample = i / 2;

		pcmBuf[sample] = ((charBuf[i + 1] & 0xFF) << 8) | (charBuf[i] & 0xFF);
	}

	ts3Functions.processCustomCaptureData("wsstream", pcmBuf, PCM_BUFFER_SIZE);
}

DWORD WINAPI MainThread(LPVOID lpParam) {
	if (ts3Functions.registerCustomDevice("wsstream", "WebSocket Stream Device", 48000, 1, 48000, 1) != ERROR_ok)
		printf("Failed to register device\n");

	wss->run(WEBSOCKET_SERVER_PORT);

	return 0;
}
#include "wsserver.h"

void on_message(websocketpp::connection_hdl hdl, message_ptr msg);

WebSocketServer::WebSocketServer() {
	_server.set_error_channels(websocketpp::log::elevel::all);
	_server.set_access_channels(websocketpp::log::alevel::all ^ websocketpp::log::alevel::frame_payload);

	_server.init_asio();
}

void WebSocketServer::run(int port) {
	printf("I'm gonna crash!\n");

	// Crash
	_server.set_message_handler(websocketpp::lib::bind(&on_message, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));

	printf("I crashed :(\n");
	Sleep(1000);

	_server.listen(port);
	_server.start_accept();
	_server.run();
}

void WebSocketServer::stop() {
	_server.stop_listening();
}
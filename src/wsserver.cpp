#include "wsserver.h"

void on_message(server *s, websocketpp::connection_hdl hdl, message_ptr msg);

WebSocketServer::WebSocketServer() {
	_server.set_error_channels(websocketpp::log::elevel::all);
	_server.set_access_channels(websocketpp::log::alevel::all ^ websocketpp::log::alevel::frame_payload);

	_server.init_asio();
}

void WebSocketServer::run(int port) {
	_server.set_message_handler(websocketpp::lib::bind(&on_message, &_server, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));

	_server.listen(port);
	_server.start_accept();
	_server.run();
}

void WebSocketServer::stop() {
	_server.stop_listening();
}
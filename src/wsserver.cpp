#include "wsserver.h"

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

void on_message(websocketpp::connection_hdl hdl, message_ptr msg);

WebSocketServer::WebSocketServer() {
	_server.set_error_channels(websocketpp::log::elevel::all);
	_server.set_access_channels(websocketpp::log::alevel::all ^ websocketpp::log::alevel::frame_payload);

	_server.init_asio();
}

void WebSocketServer::on_open(WebSocketServer *wss, websocketpp::connection_hdl client) {
	wss->clients.push_back(client);
}

void WebSocketServer::run(int port) {
	_server.set_message_handler(websocketpp::lib::bind(&on_message, ::_1, ::_2));
	_server.set_open_handler(websocketpp::lib::bind(&on_open, this, ::_1));

	_server.listen(port);
	_server.start_accept();
	_server.run();
}

void WebSocketServer::stop() {
	_server.stop_listening();

	for (websocketpp::connection_hdl const client : clients)
		_server.close(client, websocketpp::close::status::going_away, "server shutting down");
}
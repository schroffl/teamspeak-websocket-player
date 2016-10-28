#pragma once

#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_TYPE_TRAITS_

#include <list>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;
typedef server::message_ptr message_ptr;

class WebSocketServer {
	public:
		WebSocketServer();
		void run(int port);
		void stop();

		static void on_open(WebSocketServer *wss, websocketpp::connection_hdl client);

	private:
		server _server;
		std::list<websocketpp::connection_hdl> clients;
};

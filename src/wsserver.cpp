#include "wsserver.h"

void on_message(server *s, websocketpp::connection_hdl hdl, message_ptr msg);

WebSocketServer::WebSocketServer() {
	server s;
	this->m_endpoint = &s;

	this->m_endpoint->set_error_channels(websocketpp::log::elevel::all);
	this->m_endpoint->set_access_channels(websocketpp::log::alevel::all ^ websocketpp::log::alevel::frame_payload);

	this->m_endpoint->init_asio();
}

void WebSocketServer::run(int port) {
	this->m_endpoint->set_message_handler(websocketpp::lib::bind(&on_message, this->m_endpoint, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));

	this->m_endpoint->listen(port);
	this->m_endpoint->start_accept();
	this->m_endpoint->run();
}

void WebSocketServer::stop() {
	this->m_endpoint->stop_listening();
	this->m_endpoint->stop();

	printf("Stopped WebSocketServer\n");

	delete this;
}
#pragma once
// ^ dat does that the file cannot be included multiple times from the same unit (source) file

#include "ILogger.h"

#include <string>
#include <asio.hpp>
#include <asio/detail/config.hpp>
#include <cstdint>

class SMTPSock{
public:

public:
	// ctor
    SMTPSock(asio::io_context& io, ILogger& log, std::string&& clientName);

public:
	void SendStuffToServer(const std::string& ip, uint16_t port, std::string&& fromAddy, std::string&& toAddy, std::string&& body, std::function<void(bool)>);

private:
	void OnConnected(std::string&& fromAddy, std::string&& toAddy, std::string&& body, std::function<void(bool)>);
	void OnServiceReady(std::string&& fromAddy, std::string&& toAddy, std::string&& body, std::function<void(bool)>);
	void OnMailFrom(std::string&& toAddy, std::string&& body, std::function<void(bool)>, void* ctx);
	void OnRecepientTo(std::string&& body, std::function<void(bool)>);
	void OnDataPromise(std::string&& body, std::function<void(bool)>);
	void OnBodySend(std::function<void(bool)>);
	void OnDisconnect(std::function<void(bool)>);
	void GetResponseCode(char*, std::function<void(bool)>);

private:
	asio::io_context& io;
	asio::ip::tcp::socket sock;

	ILogger& log;
    std::string clientName;
};

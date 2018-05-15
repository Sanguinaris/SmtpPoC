#pragma once
// ^ dat does that the file cannot be included multiple times from the same unit (source) file

#include "ILogger.h"

#include <string>
#include <asio.hpp>
#include <asio/detail/config.hpp>
#include <cstdint>

class SMTPSock{
public:
	using SuccessCallback = ASIO_MOVE_ARG(void(*)(bool));

public:
	// ctor
    SMTPSock(asio::io_context& io, ILogger& log, std::string&& clientName);

public:
	void SendStuffToServer(const std::string& ip, uint16_t port, std::string&& fromAddy, std::string&& toAddy, std::string&& body, SuccessCallback, void* ctx);

private:
	void OnConnected(std::string&& fromAddy, std::string&& toAddy, std::string&& body, SuccessCallback, void* ctx);
	void OnServiceReady(std::string&& fromAddy, std::string&& toAddy, std::string&& body, SuccessCallback, void* ctx);
	void OnMailFrom(std::string&& toAddy, std::string&& body, SuccessCallback, void* ctx);
	void OnRecepientTo(std::string&& body, SuccessCallback, void* ctx);
	void OnDataPromise(std::string&& body, SuccessCallback, void* ctx);
	void OnBodySend(SuccessCallback, void* ctx);
	void OnDisconnect(SuccessCallback, void* ctx);
	void GetResponseCode(char*, SuccessCallback);

private:
	asio::io_context& io;
	asio::ip::tcp::socket sock;

	ILogger& log;
    std::string clientName;
};

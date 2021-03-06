#include "SMTPSock.h"

#include <array>
#include <memory>
#include <strstream>

SMTPSock::SMTPSock(asio::io_context& io, ILogger& log, std::string && clientName) : io{ io }, sock{ io }, log{log}, clientName { std::move(clientName) }
{}

void SMTPSock::SendStuffToServer(const std::string & ip, uint16_t port, std::string && fromAddy, std::string && toAddy, std::string && body, std::function<void(bool)> clbk)
{
	std::function<void(bool)> succCall = [this, fromAddy = std::move(fromAddy), toAddy = std::move(toAddy), body = std::move(body), clbk = std::move(clbk)](const auto& ec) mutable {
		if (ec)
		{
			log.Log("Failed to connect to the remote Server: " + ec.message());
			clbk(false);
			return;
		}

		std::function<void(bool)> responseCall = [this, fromAddy = std::move(fromAddy), toAddy = std::move(toAddy), body = std::move(body), clbk = std::move(clbk)](auto succ) {
			if(succ)
				OnConnected(std::move(fromAddy), std::move(toAddy), std::move(body), std::move(clbk));
			else
				log.Log("Njet Gud");
		};

		GetResponseCode("220", responseCall);
	};

	sock.async_connect({ asio::ip::address::from_string(ip), port }, succCall);
}

void SMTPSock::OnConnected(std::string && fromAddy, std::string && toAddy, std::string && body, std::function<void(bool)> clbk)
{
	auto buf = std::make_unique<std::strstreambuf>(512);
	auto& bufRef = *buf;

	sock.async_read_some(bufRef, [this, buf = std::move(buf), fromAddy = std::move(fromAddy), toAddy = std::move(toAddy), body = std::move(body), clbk = std::move(clbk)](const auto& ec, auto bytes) mutable {
		if (ec)
		{
			log.Log("Failed to get Server response: " + ec.message());
			clbk(false);
			return;
		}

		log.Log(std::to_string(bytes) + " Bytes Received...");

		auto str = buf->
			log.Log(buf->str());

		if ((*buf)[0] == '2' && (*buf)[1] == '2' && (*buf)[2] == '0')
			OnServiceReady(std::move(fromAddy), std::move(toAddy), std::move(body), std::move(clbk), ctx);
		else
			log.Log("Service not ready");
	});
}

void SMTPSock::OnServiceReady(std::string && fromAddy, std::string && toAddy, std::string && body, std::function<void(bool)> clbk)
{
	auto buf = std::make_unique<std::strstream>();
	auto& bufRef = *buf;
	bufRef << "HELO " << clientName << std::ends;

	sock.async_send(bufRef, [this, fromAddy = std::move(fromAddy), toAddy = std::move(toAddy), body = std::move(body), clbk = std::move(clbk)](const auto& ec, auto bytes) mutable {
		if (ec)
		{
			log.Log("Failed to get Server response: " + ec.message());
			clbk(false);
			return;
		}

		log.Log(std::to_string(bytes) + " Bytes Send...");
	});
}

void SMTPSock::OnMailFrom(std::string && toAddy, std::string && body, std::function<void(bool)>)
{
}

void SMTPSock::OnRecepientTo(std::string && body, std::function<void(bool)>)
{
}

void SMTPSock::OnDataPromise(std::string && body, std::function<void(bool)>)
{
}

void SMTPSock::OnBodySend(std::function<void(bool)>)
{
}

void SMTPSock::OnDisconnect(std::function<void(bool)>)
{
}

void SMTPSock::GetResponseCode(char* ctx, std::function<void(bool)> clbk)
{
	auto buf = std::make_unique<std::strstreambuf>(512);
	auto& bufRef = *buf;

	sock.async_read_some(bufRef, [this, buf = std::move(buf), clbk = std::move(clbk), ctx](const auto& ec, auto bytes) mutable {
		if (ec)
		{
			log.Log("Failed to get Server response: " + ec.message());
			clbk(false);
			return;
		}

		log.Log(std::to_string(bytes) + " Bytes Received...");

		log.Log(buf->str());

		if (buf[0] == ctx[0] && buf[1] == ctx[1] && buf[2] == ctx[2])
			clbk(true);
		else
			clbk(false);
	});
}

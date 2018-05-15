#include "SMTPSock.h"
#include "ConsoleLogger.h"

#include <iostream>
#include <future>

// Prints the help to the console
void PrintHelp()
{
	std::cout << "SmtpTest0r:\tCredz gehen raus an mich\n\n";
	std::cout << "Usage: SmtpTest0r [OPTIONS] [BODY]\n\n";
	std::cout << "-h\t--help\tJust to be sure\n";
	std::cout << "-n\t--name\tClient Name (Default: enormous.wang)\n";
	std::cout << "-s\t--server\tThe Server IP to connect to (NO DEFAULT)\n";
	std::cout << "-p\t--port\tThe Port to connect to (Default: 25)\n";
	std::cout << "-f\t--from\tThe Sender Email addy (Default: trumpe@enormous.wang)\n";
	std::cout << "-t\t--to\tThe Receiver Email addy (Default: smtptest0r@binkmail.com)\n";
}

// The argument parsing and such should be refactored into its own extendable class
int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		PrintHelp();
		return 1;
	}

	// The Network IO Object
	asio::io_context io;
	
	// DATA
	std::string ClientName = "enormous.wang", ServerIp, FromAddy = "trumpe@enormous.wang", ToAddy = "smtptest0r@binkmail.com", Body;
	uint16_t port = 25;

	// Loop over every Argument we've gotten
	for (auto i = 1; i < argc; ++i)
	{
		if (!std::strstr(argv[i], "-n") || !std::strstr(argv[i], "--name"))
		{
			if (++i < argc)
			{
				ClientName = argv[i];
			}
		}
		else if (!std::strstr(argv[i], "-s") || !std::strstr(argv[i], "--server"))
		{
			if (++i < argc)
			{
				ServerIp = argv[i];
			}
		}
		else if (!std::strstr(argv[i], "-p") || !std::strstr(argv[i], "--port"))
		{
			if (++i < argc)
			{
				port = std::atoi(argv[i]);
			}
		}
		else if (!std::strstr(argv[i], "-f") || !std::strstr(argv[i], "--from"))
		{
			if (++i < argc)
			{
				FromAddy = argv[i];
			}
		}
		else if (!std::strstr(argv[i], "-t") || !std::strstr(argv[i], "--to"))
		{
			if (++i < argc)
			{
				ToAddy = argv[i];
			}
		}
		else if (!std::strstr(argv[i], "-h") || !std::strstr(argv[i], "--help"))
		{
			PrintHelp();
		}
		else {
			Body += argv[i] + ' ';
		}
	}

	// Sanity Check
	if (ClientName.empty() || ServerIp.empty() || FromAddy.empty() || ToAddy.empty() || Body.empty() || port == 0)
	{
		PrintHelp();
		return 1;
	}

	std::promise<bool> ret;
	ConsoleLogger log;
	SMTPSock smtpReq{ io, log, std::move(ClientName) };

	smtpReq.SendStuffToServer(ServerIp, port, std::move(FromAddy), std::move(ToAddy), std::move(Body), [](auto ctx, auto succ) {
		// not gud. this is a c approach. i couldnt be bothered to write an actually move only callback type
		reinterpret_cast<std::promise<bool>*>(ctx)->set_value(succ);
	}, &ret);

	io.run();
	auto fut = ret.get_future();
	fut.wait();
    return fut.get();
}

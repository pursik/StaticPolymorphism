#pragma once

#include "Logger.h"
#include "BufferHandler.h"
#include "TCPConnection.h"


template <typename Base>
class TCPServer : public BaseTemplate<TCPServer<Base>>
{
public:
	TCPServer(boost::asio::io_context& io_context, uint64_t port, Buffer& buffer)
		: baseProcessor_(buffer)
		, logger_("TCPServer port..." + std::to_string(port))
		, io_context_(io_context)
		, acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
		, ID_(0)
	{
	}

	void ProcessData(std::span<const char> source)
	{
		throw std::logic_error("Not implemented");
	}

	bool NotifyComplete()
	{
		auto client_connection = TCPConnection::Create(io_context_, ++ID_);

		acceptor_.async_accept(
			client_connection->Socket(),
			[this, client_connection](boost::system::error_code error)
			{
				if (!error)
				{
					std::cout << "Accepted new connection" << std::endl;

					client_connection->Read(baseProcessor_);
					return NotifyComplete();
				}
				else
				{
					std::cerr << "Error during accepting connection: " << error.message() << "\n";
					return true;
				}
			});
		io_context_.run();
		return true;
	}
	std::span<const char> WaitNextData()
	{
		throw std::logic_error("Not implemented");
	}
private:
	boost::asio::ip::tcp::acceptor acceptor_;  ///< Acceptor to listen for incoming client connections
	boost::asio::io_context& io_context_;  ///< Reference to the io_context object
	Logger logger_;  ///< Logger for logging messages
	size_t ID_; ///< Unique identifier for the connection.
	Base baseProcessor_; ///< Base processor for handling data
};



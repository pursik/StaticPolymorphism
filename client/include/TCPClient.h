#pragma once

#include "Logger.h"
#include "BaseTemplate.h"
#include "VectorView.h"
#include "Config.h"
#include "Const.h"
#include <span>
#include <string>
#include <boost/asio.hpp>
#include <atomic>

using boost::asio::ip::tcp;
/**
 * @class TCPClient
 * @brief A class that represents a TCP client for connecting to a server and sending data.
 */
template <typename Base>
class TCPClient : public BaseTemplate<TCPClient<Base>>
{
public:
	TCPClient(const SCommand& command) :
		logger_("TCPClient port..." + std::to_string(command.port) + " server..." + command.serverName), baseProcessor_()
	{
		try
		{
			if (!command.sources)
			{
				throw std::logic_error("Source file name is not set");
			}
			sources = *command.sources;

			auto endpoints = resolver_.resolve(command.serverName, std::to_string(command.port));

			boost::asio::async_connect(socket_, endpoints,
				[this](boost::system::error_code ec, const tcp::endpoint&)
				{
					if (!ec)
					{
						std::cout << "Connected to server\n";
						is_connected_to_server_ = true;
					}
					else
					{
						std::cerr << "Error connecting to server: " << ec.message() << " (Code: " << ec.value() << ")\n";
					}
				});
			io_context_.run(); // Run the io_context to process asynchronous operations
		}
		catch (const std::exception& e)
		{
			std::cerr << "Exception in io_context: " << e.what() << "\n";
		}
	}

	void ProcessData(std::span<const char> data)
	{
		//std::cout << __FUNCTION__ << std::endl;
		if (!is_connected_to_server_)
		{
			std::cerr << "Cannot send data, not connected to server.\n";
			return;
		}
		if (!socket_.is_open()) {
			std::cerr << "Socket is not open. Cannot write data.\n";
			return;
		}
		// Send the data to the server asynchronously
		boost::asio::async_write(socket_, boost::asio::buffer(data.data(), data.size()),
			[this](boost::system::error_code ec, std::size_t bytes_transferred)
			{
				if (!ec)
				{
					std::cout << "Sent " << bytes_transferred << "\n";
				}
				else
				{
					std::cerr << "Error during file transmission: " << ec.message() << "\n";
					socket_.close();
				}
			});
	}

	bool NotifyComplete()
	{
		while (!WaitNextData().empty())
		{
			while (!baseProcessor_.NotifyComplete())
			{
				baseProcessor_.ProcessData({});
				const auto data = baseProcessor_.WaitNextData();

				std::vector<char> buffer{ static_cast<char>(consts::DataType::Type_File_Data) };
				buffer.insert(buffer.end(), data.begin(), data.end());
				ProcessData(buffer);
			}
		}
		return !socket_.is_open();
	}
	std::span<const char> WaitNextData()
	{
		auto source = sources.next();
		if (source != sources.end())
		{
			baseProcessor_.ProcessData({ source->data(), source->size() });
			const auto data = baseProcessor_.WaitNextData();

			std::vector<char> buffer{ static_cast<char>(consts::DataType::Type_File_Name) };
			buffer.insert(buffer.end(), data.begin(), data.end());
			ProcessData(buffer);

			return { source->data(), source->size() };
		}
		return {};
	}
private:
	boost::asio::io_context io_context_;///<to initiate and manage asynchronous operations, such as connecting to a server, reading, or writing data over a network.
	tcp::socket socket_{ io_context_ };  ///< TCP socket for communication
	tcp::resolver resolver_{ io_context_ };  ///< Resolver for DNS resolution
	std::atomic<bool> is_connected_to_server_{ false };  ///< Thread-safe flag to indicate if data can be sent
	Logger logger_;  ///< Logger for logging messages
	Base baseProcessor_; ///< Base processor for handling data
	VectorView<std::string, std::allocator<std::string>> sources;
};

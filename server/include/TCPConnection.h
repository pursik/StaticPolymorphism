#pragma once

#include "Logger.h"
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class TCPConnection : public std::enable_shared_from_this<TCPConnection>
{
public:
	static std::shared_ptr<TCPConnection> Create(boost::asio::io_context& io_context, size_t ID)
	{
		return std::shared_ptr<TCPConnection>(new TCPConnection(io_context, ID));
	}

	tcp::socket& Socket()
	{
		return socket_;
	}

	template <typename T>
	void Read(T& processor)
	{
		auto self = shared_from_this();

		self->buffer_.clear();
		self->buffer_.push_back(static_cast<char>(ID_));

		socket_.async_read_some(
			boost::asio::buffer(self->buffer_.data() + consts::IDBytes, consts::BufferSize - 1),
			[self, &processor](const boost::system::error_code& error, std::size_t length)
			{
				if (!error)
				{
					std::cout << "Message received from client " << self->ID_ << " size..."<< length << "\n";
					processor.ProcessData(std::span<const char>(self->buffer_.data(), length + consts::IDBytes));
					self->Read(processor);
				}
				else if (error == boost::asio::error::eof)
				{
					std::cout << "Client disconnected ID: " << self->ID_ << "\n";
					self->socket_.close();
				}
				else if (error == boost::asio::error::operation_aborted)
				{
					std::cout << "Operation aborted\n";
				}
				else
				{
					std::cerr << "Error while receiving message: " << error.message() << "\n";
				}
			});
	}
private:
	explicit TCPConnection(boost::asio::io_context& io_context, size_t ID)
		: logger_("TCPConnection "),
		socket_(io_context),
		ID_(ID)
	{
		buffer_.reserve(consts::BufferSize);
	}

	tcp::socket socket_; ///< The socket associated with the connection.
	Logger logger_; ///< Logger instance for logging
	std::vector<char> buffer_;/// Buffer to store the received data.
	size_t ID_; ///< Unique identifier for the connection.
};

#include "TaskFactory.h"
#include "TCPServer.h"
#include "CommandFactory.h"
#include "Config.h"
#include "Reader.h"
#include "FileWriter.h"
#include "Serializer.h"
#include <thread>
#include "Decryptor.h"

void TaskFactory::Run(int argc, const char* argv[])
{
	boost::asio::io_context io_context;
	const auto command = CommandFactory::Create(argc, argv);
	Buffer buffer(consts::BufferSize);

	auto server = TCPServer<BufferHandler>(io_context, command.port, buffer);
	std::thread readerThread([&server]() {server.NotifyComplete();});
	
	if (command.crypto)
	{
		Reader<Decryptor<Serializer<FileWriter<BufferHandler>>>> dataHandler(buffer);
		std::thread writerThread([&dataHandler]() {dataHandler.NotifyComplete(); });
		readerThread.join();
		writerThread.join();
	}
	else
	{

		Reader<Serializer<FileWriter<BufferHandler>>> dataHandler(buffer);
		std::thread writerThread([&dataHandler]() {dataHandler.NotifyComplete(); });
		readerThread.join();
		writerThread.join();
	}
	
}

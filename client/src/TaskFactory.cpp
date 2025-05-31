#include "TaskFactory.h"
#include "CommandFactory.h"
#include "TCPClient.h"
#include "Reader.h"
#include "Serializer.h"
#include "Encryptor.h"

void TaskFactory::Run(int argc, const char* argv[])
{
	auto command = CommandFactory::Create(argc, argv);
	if (command.crypto)
	{
		auto task = TCPClient<Reader<Encryptor<Serializer>>>(command);
		task.NotifyComplete();
	}
	else
	{
		auto task = TCPClient<Reader<Serializer>>(command);
		task.NotifyComplete();
	}
}



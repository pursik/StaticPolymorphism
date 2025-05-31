#pragma once

class TaskFactory
{
public:
	TaskFactory() = delete;
	TaskFactory(const TaskFactory&) = delete;
	TaskFactory& operator=(const TaskFactory&) = delete;
	TaskFactory(TaskFactory&&) = delete;
	TaskFactory& operator=(TaskFactory&&) = delete;

	static void Run(int argc, const char* argv[]);
};
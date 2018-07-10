#pragma once

#include <iostream>
#include <fstream>
#include <mutex>
#include <string>

class DebugMessageHandler
{
private:
	DebugMessageHandler()
	{
		outputFile.open("DebugMessages.txt", std::ios::out);
		outputFile << "";
		outputFile.close();
	}
	DebugMessageHandler(DebugMessageHandler const&);              // Don't Implement.
	void operator=(DebugMessageHandler const&) {} // Don't implement

	static bool instanceFlag;
	static DebugMessageHandler* instance;
	std::mutex mutexLock;
	std::ofstream outputFile;

	std::string storedMessages;

public:
	static DebugMessageHandler* GetInstance();
	static void DestroyInstance();

	~DebugMessageHandler()
	{
		instanceFlag = false;
	}

	void StoreMessage(const char* message);
	void Log(const char* message);
};
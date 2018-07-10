#include "../Headers/DebugMessageHandler.h"

bool DebugMessageHandler::instanceFlag = false;
DebugMessageHandler* DebugMessageHandler::instance = nullptr;

DebugMessageHandler* DebugMessageHandler::GetInstance()
{
	if (!instanceFlag)
	{
		instance = new DebugMessageHandler();
		instanceFlag = true;
		return instance;
	}
	else
	{
		return instance;
	}
}

void DebugMessageHandler::DestroyInstance()
{
	if (instanceFlag)
	{
		instanceFlag = false;
		delete instance;
	}
}

void DebugMessageHandler::StoreMessage(const char* message)
{
	mutexLock.lock();
	storedMessages += message ;
	storedMessages += '\n';
	mutexLock.unlock();
}

void DebugMessageHandler::Log(const char* message)
{
	mutexLock.lock();
	outputFile.open("DebugMessages.txt", std::ios::out | std::ios::app);
	outputFile << storedMessages;
	outputFile << message << "\n";
	outputFile.close();
	storedMessages.clear();
	mutexLock.unlock();
}
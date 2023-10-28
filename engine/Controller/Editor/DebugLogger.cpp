#include "DebugLogger.h"

DebugLogger::DebugLogger() :
	streamCapture(std::cout)
{
}

DebugLogger::~DebugLogger()
{
}

void DebugLogger::Log(unsigned int type, const std::string message)
{
	switch (type)
	{
	case GAEM_ERROR:
		std::cout << "[Gaem ERROR--][" <<GetTimeStamp() << "]: " << message << "\n";
		break;
	case GAEM_LOG:
		std::cout << "[Gaem LOG----][" << GetTimeStamp() << "]: " << message << "\n";
		break;
	case GAEM_DEBUG:
		std::cout << "[Gaem DEBUG--][" << GetTimeStamp() << "]: " << message << "\n";
		break;
	case GAEM_WARNING:
		std::cout << "[Gaem WARNING][" << GetTimeStamp() << "]: " << message << "\n";
		break;
	default:
		break;
	}
}

std::string DebugLogger::GetTimeStamp()
{
	auto now = std::chrono::system_clock::now();

	std::time_t time = std::chrono::system_clock::to_time_t(now);
	static std::stringstream ss;

	ss.str("");
	ss.clear();

	ss << std::put_time(std::localtime(&time), "%H:%M:%S");

	return ss.str();
}

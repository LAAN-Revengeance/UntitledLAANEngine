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
		std::cout << "[Gaem ERR][" <<GetTimeStamp() << "]" << message << "\n";
		break;
	case GAEM_LOG:
		std::cout << "[Gaem LOG][" << GetTimeStamp() << "]" << message << "\n";
		break;
	case GAEM_DEBUG:
		std::cout << "[Gaem DBG][" << GetTimeStamp() << "]" << message << "\n";
		break;
	case GAEM_WARNING:
		std::cout << "[Gaem WRN][" << GetTimeStamp() << "]" << message << "\n";
		break;
	default:
		break;
	}
}

void DebugLogger::Log(unsigned int type, const std::string message, GameObject* obj)
{
	if (obj) {
		auto fullMessage = "[" + obj->name + "]: " + message;
		Log(type, fullMessage);
	}else{
		Log(type, message);
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

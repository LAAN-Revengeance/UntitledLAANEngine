#include "DebugLogger.h"


bool DebugLogger::log_err		= true;
bool DebugLogger::log_log		= true;
bool DebugLogger::log_dbg		= true;
bool DebugLogger::log_wrn		= true;


DebugLogger::DebugLogger() :
	streamCapture(std::cout)
{
}

DebugLogger::~DebugLogger()
{
}

void DebugLogger::Log(unsigned int type, const std::string message)
{
	if		(log_err && type == GAEM_ERROR)		{ std::cout << "[Gaem ERR][" << GetTimeStamp() << "]" << message << "\n"; }
	else if (log_log && type == GAEM_LOG)		{ std::cout << "[Gaem LOG][" << GetTimeStamp() << "]" << message << "\n"; }
	else if (log_dbg && type == GAEM_DEBUG)		{ std::cout << "[Gaem DBG][" << GetTimeStamp() << "]" << message << "\n"; }
	else if (log_wrn && type == GAEM_WARNING)	{ std::cout << "[Gaem WRN][" << GetTimeStamp() << "]" << message << "\n"; }
}

void DebugLogger::Log(unsigned int type, const std::string message, std::string name)
{
	if (!name.empty()) {
		auto fullMessage = "[" + name + "]:" + message;
		Log(type, fullMessage);
	}else{
		Log(type, message);
	}
}

void DebugLogger::Clear()
{
	streamCapture.Clear();
}

void DebugLogger::SetLogLevel(unsigned int type, bool isActive)
{
	if			(type == GAEM_ERROR)		{ log_err = isActive; }
	else if		(type == GAEM_LOG)			{ log_log = isActive; }
	else if		(type == GAEM_DEBUG)		{ log_dbg = isActive; }
	else if		(type == GAEM_WARNING)		{ log_wrn = isActive; }
}

bool DebugLogger::GetLogLevel(unsigned int type)
{
	if		(type == GAEM_ERROR)	{ return log_err; }
	else if (type == GAEM_LOG)		{ return log_log; }
	else if (type == GAEM_DEBUG)	{ return log_dbg; }
	else if (type == GAEM_WARNING)	{ return log_wrn; }
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

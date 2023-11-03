#pragma once
#include <string>
#include "StreamCapture.h"
#include <chrono>
#include <iomanip>

const unsigned int GAEM_ERROR	= 0;
const unsigned int GAEM_LOG		= 1;
const unsigned int GAEM_DEBUG	= 2;
const unsigned int GAEM_WARNING	= 3;
const unsigned int GAEM_ALL		= 4;

class DebugLogger
{
public:
	DebugLogger();
	~DebugLogger();


	static void Log(unsigned int type,const std::string message);
	static void Log(unsigned int type,const std::string message, std::string name);

	void Clear();

	static void SetLogLevel(unsigned int type, bool isActive);
	static bool GetLogLevel(unsigned int type);

	StreamCapture streamCapture;
private:

	static bool log_err;
	static bool log_log;
	static bool log_dbg;
	static bool log_wrn;

	static std::string GetTimeStamp();
};

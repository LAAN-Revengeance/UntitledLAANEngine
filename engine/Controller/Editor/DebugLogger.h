#pragma once
#include <string>
#include "StreamCapture.h"
#include <chrono>
#include <iomanip>

const unsigned int GAEM_ERROR	= 0;
const unsigned int GAEM_LOG		= 1;
const unsigned int GAEM_DEBUG	= 2;
const unsigned int GAEM_WARNING	= 3;

class DebugLogger
{
public:
	DebugLogger();
	~DebugLogger();

	static void Log(unsigned int type,const std::string message);

	StreamCapture streamCapture;
private:

	static std::string GetTimeStamp();
};

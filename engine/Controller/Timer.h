#pragma once
#include <GLFW/glfw3.h>

class Timer
{
public:
	static Timer& Get();

	double Time();

private:

	Timer();
	~Timer();
	///Copy constructor removed becuase singleton
	Timer(const Timer&) = delete;
	///Assignment operator removed becuase singleton
	Timer& operator = (const Timer&) = delete;
};


#include "Timer.h"

 Timer& Timer::Get() {
	Timer t_Instance;
	return t_Instance;
}

double Timer::Time() {
	return glfwGetTime();
}

Timer::Timer()
{
}

Timer::~Timer()
{
}

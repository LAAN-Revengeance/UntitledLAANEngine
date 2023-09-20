#include "Timer.h"

 void Timer::Update()
 {
	 double currentTime = Time();
	 deltaTime = currentTime - lastTime;
	 lastTime = currentTime;
 }

 double Timer::DeltaTime()
 {
	 return deltaTime;
 }

double Timer::Time() {
	auto timeNow = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> timeTotal = timeNow - startTime;
	return timeTotal.count();
}

void Timer::Reset()
{
	startTime = std::chrono::high_resolution_clock::now();
}

Timer::Timer():
	startTime(std::chrono::high_resolution_clock::now()),
	deltaTime(0), 
	lastTime(0) 
{}

Timer::~Timer()
{

}

#pragma once
#include <chrono>

/**
*	@Class Timer
*	@brief Track time since its construction and time in between updates
*
*	@author Andres Comeros-Ochtman
*	@version 2.0
*	@date 20/09/2023
*/
class Timer
{
public:

	Timer();
	~Timer();

	///Call at the end start of every update cycle
	void Update();
	///Return elapsed time in seconds since last update call
	double DeltaTime();
	///Return elapsed time in seconds since timer started or reset was called
	double Time();
	///Restart this timer
	void Reset();
private:

	///previous time between frames
	double lastTime = 0.0f;
	///time since last frame
	double deltaTime = 0.0f;
	///Start time
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
};


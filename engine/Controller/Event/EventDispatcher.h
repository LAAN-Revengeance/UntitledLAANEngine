#pragma once
#include "Event.h"
#include <functional>
#include <map>
#include <vector>

using EventCallback = std::function<void(const Event&)>;

class EventDispatcher
{
public:

	//Execute all callbacks registered to the event
	void Post(const Event& event);

	//Subscribe a cellback to an event type
	void Subscribe(EventDescriptor, EventCallback&& callback);

private:

	std::map<EventDescriptor, std::vector<EventCallback>> observers;
};
#pragma once

#include "Event.h"
#include <functional>
#include <map>
#include <vector>

namespace GaemEvents {
	using EngineEventCallback = std::function<void(const Event&)>;

	class EventDispatcher
	{
	public:

		//Execute all callbacks registered to the event
		void Post(const Event& event);

		//Subscribe a callback to an event type
		void Subscribe(EventDescriptor, EngineEventCallback&& callback);

	private:

		std::map<EventDescriptor, std::vector<EngineEventCallback>> observers;
	};
}
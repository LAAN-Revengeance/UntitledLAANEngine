#include "EventDispatcher.h"
namespace GaemEvents {
	void EventDispatcher::Post(const Event& event)
	{
		EventDescriptor type = event.GetType();
		if (observers.find(type) == observers.end())
			return;

		for (auto&& observer : observers[type])
			observer(event);
	}

	void EventDispatcher::Subscribe(EventDescriptor descriptor, EngineEventCallback&& callback)
	{
		observers[descriptor].push_back(callback);
	}
}


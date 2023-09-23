#pragma once

using EventDescriptor = const char*;

class Event
{
public:
	virtual ~Event() {}

	virtual EventDescriptor GetType() const = 0;

private:
};

class EventWindowResize : public Event {

	EventWindowResize(int w, int h) {
		width = w;
		height = h;
	}

	EventDescriptor GetType() { return descriptor; }

	static constexpr EventDescriptor descriptor = "windowResize";

	int width;
	int height;
};
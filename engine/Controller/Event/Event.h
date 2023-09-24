#pragma once
#include <iostream>
using EventDescriptor = const char*;

class Event
{
public:
	virtual ~Event() {}

	virtual EventDescriptor GetType() const = 0;

private:
};

class WindowResizeEvent : public Event {

public:
	WindowResizeEvent(int w, int h) {
		width = w;
		height = h;
	}
	~WindowResizeEvent() {}

	EventDescriptor GetType()const override{ return descriptor; }

	static constexpr EventDescriptor descriptor = "windowResize";

	int width;
	int height;
};
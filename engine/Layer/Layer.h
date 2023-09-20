#pragma once

class Layer
{
public:
	Layer() {};
	virtual ~Layer() {};

	virtual void OnAttatch() = 0;
	virtual void OnDetatch() = 0;
	virtual void OnUpdate(double deltaTime) = 0;
	virtual void OnDraw(double deltaTime) = 0;
private:
};

#pragma once
#include "Layer.h"
#include <vector>

class LayerStack
{
public:
	LayerStack();
	~LayerStack();

	void Push(Layer* nLayer);
	Layer* Pop();

	std::vector<Layer*>::iterator begin();
	std::vector<Layer*>::iterator end();

private:
	std::vector<Layer*> layers;
	std::vector<Layer*>::iterator insertIndex;
};
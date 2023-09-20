#include "LayerStack.h"

LayerStack::LayerStack()
{
	insertIndex = layers.begin();
}

LayerStack::~LayerStack()
{
	for (auto* it : layers)
		delete it;
}

void LayerStack::Push(Layer* nLayer)
{
	layers.emplace_back(nLayer);
}

Layer* LayerStack::Pop()
{
	Layer* rLayer = layers.back();
	layers.pop_back();
	return rLayer;
}

std::vector<Layer*>::iterator LayerStack::begin()
{
	return layers.begin();
}

std::vector<Layer*>::iterator LayerStack::end()
{
	return layers.end();
}

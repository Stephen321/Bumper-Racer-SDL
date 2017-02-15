#pragma once

#include "Component.h"
#include "GraphNode.h"
typedef GraphNode<string> Node;

class AIComponent : public Component
{
public:
	AIComponent()
		: Component::Component(Component::Type::AI),
		path(vector<Node*>()),
		updateRate(0.f),
		updateTimer(0),
		callAstar(false)
	{
	}

	~AIComponent()
	{
	}

public:
	vector<Node*> path;
	float updateRate;
	float updateTimer;
	bool callAstar;
};

#pragma once

#include "Component.h"

#include <vector>

class Entity
{
public:
	enum class Type : unsigned short
	{ //draw order is determined by the below order
		Flag = 0x0001,  //1
		Checkpoint = 0x0002, // 10
		Wall = 0x0004, // 100
		Tile = 0x0008, //1000
		Player = 0x0010 //10000  
	};

public:
								Entity(Type type);
								~Entity();

	void						AddComponent(Component* component);
	void						RemoveComponent(Component::Type componentType);

	Component*					GetComponent(Component::Type componentType);
	std::vector<Component*>&	GetComponents();
	Type						GetType() const;

private:
	std::vector<Component*>		_components;
	Type						_type;
};


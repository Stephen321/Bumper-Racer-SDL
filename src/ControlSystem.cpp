#include "ControlSystem.h"

#include "PhysicsComponent.h"
#include "BoundsComponent.h"

#include <iostream>

ControlSystem::ControlSystem(Camera2D::Camera* camera, float updateRate)
	: System(updateRate)
	, _camera(camera)
{
	
}


ControlSystem::~ControlSystem()
{
}

void ControlSystem::Process(float dt)
{
	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;

		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* e : (*it).second)
			{
				BoundsComponent* bound = static_cast<BoundsComponent*>(e->GetComponent(Component::Type::Bounds));
				
				//initialize mouuse position
				SDL_Point mouse;
				SDL_GetMouseState(&mouse.x, &mouse.y);

				//calculate mouse position to world
				Camera2D::Point convertedPoint = _camera->screenToWorld(Camera2D::Point(mouse.x, mouse.y));

				Camera2D::Point difference = { convertedPoint.x - bound->rect.x + bound->origin.x, convertedPoint.y - bound->rect.y + bound->origin.y };
				bound->angle = atan2(difference.y, difference.x) * 180.f / M_PI;
			}
		}
	}
}

void ControlSystem::MovePlayer(int x, int y, Entity*& entity)
{
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(entity->GetComponent(Component::Type::Physics));
	physics->xVelocity = x * 5;
	physics->yVelocity = y * 5;

	BoundsComponent* bounds = static_cast<BoundsComponent*>(entity->GetComponent(Component::Type::Bounds));
	bounds->rect.x += physics->xVelocity;
	bounds->rect.y += physics->yVelocity;
	bounds->rect.x = (int)bounds->rect.x;
	bounds->rect.y = (int)bounds->rect.y;
}

void ControlSystem::OnEvent(Event evt)
{

}

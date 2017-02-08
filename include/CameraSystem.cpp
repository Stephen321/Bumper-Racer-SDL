#include "CameraSystem.h"

CameraSystem::CameraSystem()
{
}

CameraSystem::~CameraSystem()
{
}

void CameraSystem::Process(float dt)
{
	for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
	{
		for (Entity* e : (*it).second)
		{
			BoundsComponent* bound = static_cast<BoundsComponent*>(e->GetComponent(Component::Type::Bounds));
			_camera.setCentre(bound->x + bound->rect.w * 0.5f, bound->rect.y + bound->rect.h * 0.5f);
		}
	}
}

void CameraSystem::Init(int w, int h)
{

	_camera.init(w, h);
}
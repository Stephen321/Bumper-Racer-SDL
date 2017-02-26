#include "CreationSystem.h"

#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "CheckpointComponent.h"

#include "BasicTypes.h"
#include "Helpers.h"

#include <iostream>


CreationSystem::CreationSystem(std::vector<std::pair<EntityType, std::vector<float>>>& creationRequests)
	: _creationRequests(creationRequests)
{

}

CreationSystem::~CreationSystem()
{
	for (int i = 0; i < _entities.size(); i++)
	{
		if (_entities.at(i) != nullptr)
		{
			delete _entities.at(i);
		}
	}

	_entities.clear();

}


void CreationSystem::Initialize(EntityFactory* entityFactory, BodyFactory* bodyFactory)
{
	_entityFactory = entityFactory;
	_bodyFactory = bodyFactory;
}

void CreationSystem::Process(float dt)
{
	int index = _creationRequests.size()-1;

	while (index != -1)
	{
		switch (_creationRequests.at(index).first)
		{
		case EntityType::Tile:
			_systemCreatedEntities.push_back(SetupTileEntity(_creationRequests.at(index)));
			_entities.push_back(_systemCreatedEntities.back().second);
			break;
		case EntityType::Checkpoint:
			_systemCreatedEntities.push_back(SetupCheckpointEntity(_creationRequests.at(index)));
			_entities.push_back(_systemCreatedEntities.back().second);
			break;
		case EntityType::PowerUp:
			_systemCreatedEntities.push_back(SetupPowerUpEntity(_creationRequests.at(index)));
			_entities.push_back(_systemCreatedEntities.back().second);
			break;
		case EntityType::AI:
			_systemCreatedEntities.push_back(SetupAIEntity(_creationRequests.at(index)));
			_entities.push_back(_systemCreatedEntities.back().second);
			break;
		case EntityType::Player:
			_systemCreatedEntities.push_back(SetupPlayerEntity(_creationRequests.at(index)));
			_entities.push_back(_systemCreatedEntities.back().second);
			break;
		case EntityType::Bullet:
			_systemCreatedEntities.push_back(SetupBulletEntity(_creationRequests.at(index)));
			_entities.push_back(_systemCreatedEntities.back().second);
			break;
		case EntityType::Weapon:
			_systemCreatedEntities.push_back(SetupWeaponEntity(_creationRequests.at(index)));
			_entities.push_back(_systemCreatedEntities.back().second);
			break;
		case EntityType::Flag:
			_systemCreatedEntities.push_back(SetupFlagEntity(_creationRequests.at(index)));
			_entities.push_back(_systemCreatedEntities.back().second);
			break;
		case EntityType::UI:
			_systemCreatedEntities.push_back(SetupUIEntity(_creationRequests.at(index)));
			_entities.push_back(_systemCreatedEntities.back().second);
			break;

		default:
			std::cout << "Object to create not within the reach of a switch statement - CreationSystem.cpp" << std::endl;
			break;
		}

		index--;
	}

	_creationRequests.clear();
}

std::pair<std::vector<SystemType>, Entity*> CreationSystem::SetupWeaponEntity(const std::pair<EntityType, std::vector<float>>& information)
{
	Entity* weapon = _entityFactory->CreateEntity(EntityType::Weapon, information.second[0]);

	TransformComponent* transform = static_cast<TransformComponent*>(weapon->GetComponent(Component::Type::Transform));

	int index = 1;
	SetupPosition(transform, information.second, index);
	//SetupSize(transform, information.second, index);


	std::vector<SystemType> systemTypes = std::vector<SystemType>();

	systemTypes.push_back(SystemType::Render);
	systemTypes.push_back(SystemType::Gun);
	systemTypes.push_back(SystemType::Destruction);

	std::pair<std::vector<SystemType>, Entity*> toBeCreated(systemTypes, weapon);

	return toBeCreated;
}
std::pair<std::vector<SystemType>, Entity*> CreationSystem::SetupPlayerEntity(const std::pair<EntityType, std::vector<float>>& information)
{
	Entity* player = _entityFactory->CreateEntity(EntityType::Player, information.second[0]);

	ColliderComponent* collider = static_cast<ColliderComponent*>(player->GetComponent(Component::Type::Collider));
	TransformComponent* transform = static_cast<TransformComponent*>(player->GetComponent(Component::Type::Transform));

	int index = 1;
	SetupPosition(transform, information.second, index);
	SetupSize(transform, information.second, index);

	collider->body = _bodyFactory->CreateBoxBody(
		  b2BodyType::b2_dynamicBody
		, b2Vec2(transform->rect.x - transform->origin.x * transform->scaleX, transform->rect.y - transform->origin.x * transform->scaleY)
		, b2Vec2(transform->rect.w / 2, transform->rect.h / 2)
		, (uint16)player->GetType()
		, PLAYER_MASK
		, false);

	collider->body->SetUserData(player);
	collider->body->SetFixedRotation(true);

	std::vector<SystemType> systemTypes = std::vector<SystemType>();

	systemTypes.push_back(SystemType::Render);
	systemTypes.push_back(SystemType::Physics);
	systemTypes.push_back(SystemType::Camera);
	systemTypes.push_back(SystemType::World);
	systemTypes.push_back(SystemType::StatusEffect);

	std::pair<std::vector<SystemType>, Entity*> toBeCreated(systemTypes, player);

	return toBeCreated;
}
std::pair<std::vector<SystemType>, Entity*> CreationSystem::SetupAIEntity(const std::pair<EntityType, std::vector<float>>& information)
{
	Entity* ai = _entityFactory->CreateEntity(EntityType::AI, information.second[0]);

	ColliderComponent* collider = static_cast<ColliderComponent*>(ai->GetComponent(Component::Type::Collider));
	TransformComponent* transform = static_cast<TransformComponent*>(ai->GetComponent(Component::Type::Transform));

	int index = 1;
	SetupPosition(transform, information.second, index);
	SetupSize(transform, information.second, index);

	collider->body = _bodyFactory->CreateBoxBody(
		b2BodyType::b2_dynamicBody
		, b2Vec2(transform->rect.x - transform->origin.x * transform->scaleX, transform->rect.y - transform->origin.x * transform->scaleY)
		, b2Vec2(transform->rect.w / 2, transform->rect.h / 2)
		, (uint16)ai->GetType()
		, AI_MASK
		, false);

	collider->body->SetUserData(ai);
	collider->body->SetFixedRotation(true);

	std::vector<SystemType> systemTypes = std::vector<SystemType>();

	systemTypes.push_back(SystemType::Render);
	systemTypes.push_back(SystemType::Physics);
	systemTypes.push_back(SystemType::AI);
	systemTypes.push_back(SystemType::StatusEffect);

	std::pair<std::vector<SystemType>, Entity*> toBeCreated(systemTypes, ai);

	return toBeCreated;
}
std::pair<std::vector<SystemType>, Entity*> CreationSystem::SetupPowerUpEntity(const std::pair<EntityType, std::vector<float>>& information)
{
	Entity* powerUp = _entityFactory->CreateEntity(EntityType::PowerUp, information.second[0]);

	ColliderComponent* collider = static_cast<ColliderComponent*>(powerUp->GetComponent(Component::Type::Collider));
	TransformComponent* transform = static_cast<TransformComponent*>(powerUp->GetComponent(Component::Type::Transform));
	SpriteComponent* spriteComponent = static_cast<SpriteComponent*>(powerUp->GetComponent(Component::Type::Sprite));

	int index = 1;
	SetupPosition(transform, information.second, index);
	//SetupSize(transform, information.second, index);

	spriteComponent->sourceRect.x *= transform->rect.w;
	spriteComponent->sourceRect.y *= transform->rect.h;
	spriteComponent->sourceRect.w = transform->rect.w;
	spriteComponent->sourceRect.h = transform->rect.h;

	collider->body = _bodyFactory->CreateBoxBody(
		b2BodyType::b2_dynamicBody
		, b2Vec2(transform->rect.x, transform->rect.y)
		, b2Vec2(transform->origin.x, transform->origin.y)
		, (uint16)powerUp->GetType()
		, POWERUP_MASK
		, true);

	collider->body->SetUserData(powerUp);
	collider->body->SetFixedRotation(true);

	std::vector<SystemType> systemTypes = std::vector<SystemType>();

	systemTypes.push_back(SystemType::Render);
	systemTypes.push_back(SystemType::Destruction);

	std::pair<std::vector<SystemType>, Entity*> toBeCreated(systemTypes, powerUp);

	return toBeCreated;
}
std::pair<std::vector<SystemType>, Entity*> CreationSystem::SetupBulletEntity(const std::pair<EntityType, std::vector<float>>& information)
{
	Entity* bullet = _entityFactory->CreateEntity(EntityType::Bullet, information.second[0]);

	ColliderComponent* collider = static_cast<ColliderComponent*>(bullet->GetComponent(Component::Type::Collider));
	TransformComponent* transform = static_cast<TransformComponent*>(bullet->GetComponent(Component::Type::Transform));
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(bullet->GetComponent(Component::Type::Physics));

	int index = 1;
	SetupPosition(transform, information.second, index);
	SetupSize(transform, information.second, index);
	SetupAngle(transform, information.second, index);
	SetupVelocity(physics, information.second, index);

	transform->origin = { (int)(transform->rect.w * 0.5f), (int)(transform->rect.h * 0.5f) };

	collider->body = _bodyFactory->CreateBoxBodyWithSensor(
		b2BodyType::b2_dynamicBody
		, b2Vec2(transform->rect.x, transform->rect.y)
		, b2Vec2(transform->origin.x, transform->origin.y)
		, (uint16)bullet->GetType()
		, BULLET_SENSOR_MASK //fix later for not selfharm
		, BULLET_BODY_MASK);

	collider->body->SetUserData(bullet);
	collider->body->SetFixedRotation(true);

	std::vector<SystemType> systemTypes = std::vector<SystemType>();

	systemTypes.push_back(SystemType::Render);
	systemTypes.push_back(SystemType::Physics);
	systemTypes.push_back(SystemType::Destruction);

	std::pair<std::vector<SystemType>, Entity*> toBeCreated(systemTypes, bullet);

	return toBeCreated;
}
std::pair<std::vector<SystemType>, Entity*> CreationSystem::SetupCheckpointEntity(const std::pair<EntityType, std::vector<float>>& information)
{
	Entity* checkpoint = _entityFactory->CreateEntity(EntityType::Checkpoint, information.second[0]);

	ColliderComponent* collider = static_cast<ColliderComponent*>(checkpoint->GetComponent(Component::Type::Collider));
	TransformComponent* transform = static_cast<TransformComponent*>(checkpoint->GetComponent(Component::Type::Transform));

	int index = 1;
	SetupPosition(transform, information.second, index);
	SetupSize(transform, information.second, index);

	transform->origin = { (int)(transform->rect.w*0.5f), (int)(transform->rect.h*0.5f) };
	transform->rect = { (int)(transform->rect.x + transform->origin.x * transform->scaleX)
					  , (int)(transform->rect.y + transform->origin.y * transform->scaleY)
					  , (int)transform->rect.w
					  , (int)transform->rect.h };

	collider->body = _bodyFactory->CreateBoxBody(
		b2BodyType::b2_staticBody
		, b2Vec2(transform->rect.x, transform->rect.y)
		, b2Vec2(transform->origin.x, transform->origin.y)
		, (uint16)checkpoint->GetType()
		, CHECKPOINT_MASK
		, true);

	collider->body->SetUserData(checkpoint);
	collider->body->SetFixedRotation(true);

	std::vector<SystemType> systemTypes = std::vector<SystemType>();

	systemTypes.push_back(SystemType::Render);

	std::pair<std::vector<SystemType>, Entity*> toBeCreated(systemTypes, checkpoint);

	return toBeCreated;
}
std::pair<std::vector<SystemType>, Entity*> CreationSystem::SetupFlagEntity(const std::pair<EntityType, std::vector<float>>& information)
{
	Entity* flag = _entityFactory->CreateEntity(EntityType::Flag, information.second[0]);

	ColliderComponent* collider = static_cast<ColliderComponent*>(flag->GetComponent(Component::Type::Collider));
	TransformComponent* transform = static_cast<TransformComponent*>(flag->GetComponent(Component::Type::Transform));

	int index = 1;
	SetupPosition(transform, information.second, index);
	SetupSize(transform, information.second, index);

	transform->origin = { (int)(transform->rect.w*0.5f), (int)(transform->rect.h*0.5f) };
	transform->rect = { (int)(transform->rect.x + transform->origin.x * transform->scaleX)
					  , (int)(transform->rect.y + transform->origin.y * transform->scaleY)
					  , (int)transform->rect.w
					  , (int)transform->rect.h };

	collider->body = _bodyFactory->CreateBoxBodyWithSensor(
		b2BodyType::b2_dynamicBody
		, b2Vec2(transform->rect.x, transform->rect.y)
		, b2Vec2(transform->origin.x, transform->origin.y)
		, (uint16)flag->GetType()
		, FLAG_SENSOR_MASK
		, FLAG_BODY_MASK);

	collider->body->SetUserData(flag);
	collider->body->SetFixedRotation(true);

	std::vector<SystemType> systemTypes = std::vector<SystemType>();

	systemTypes.push_back(SystemType::Render);
	systemTypes.push_back(SystemType::Physics);
	systemTypes.push_back(SystemType::Collision);

	std::pair<std::vector<SystemType>, Entity*> toBeCreated(systemTypes, flag);

	return toBeCreated;
}
std::pair<std::vector<SystemType>, Entity*> CreationSystem::SetupTileEntity(const std::pair<EntityType, std::vector<float>>& information)
{
	Entity* tile = _entityFactory->CreateEntity(EntityType::Tile, information.second[0]);

	TransformComponent* transform = static_cast<TransformComponent*>(tile->GetComponent(Component::Type::Transform));
	SpriteComponent* spriteComponent = static_cast<SpriteComponent*>(tile->GetComponent(Component::Type::Sprite));

	int index = 1;
	SetupPosition(transform, information.second, index);
	SetupSize(transform, information.second, index);

	spriteComponent->sourceRect.x *= transform->rect.w;
	spriteComponent->sourceRect.y *= transform->rect.h;
	spriteComponent->sourceRect.w = transform->rect.w;
	spriteComponent->sourceRect.h = transform->rect.h;


	std::vector<SystemType> systemTypes = std::vector<SystemType>();

	systemTypes.push_back(SystemType::Render);

	std::pair<std::vector<SystemType>, Entity*> toBeCreated(systemTypes, tile);

	return toBeCreated;
}
std::pair<std::vector<SystemType>, Entity*> CreationSystem::SetupUIEntity(const std::pair<EntityType, std::vector<float>>& information)
{
	Entity* uiEntity = _entityFactory->CreateEntity(EntityType::UI, information.second[0]);

	TransformComponent* transform = static_cast<TransformComponent*>(uiEntity->GetComponent(Component::Type::Transform));

	int index = 1;
	SetupPosition(transform, information.second, index);
	SetupSize(transform, information.second, index);

	std::vector<SystemType> systemTypes = std::vector<SystemType>();

	systemTypes.push_back(SystemType::UI);

	std::pair<std::vector<SystemType>, Entity*> toBeCreated(systemTypes, uiEntity);

	return toBeCreated;
}

void CreationSystem::SetupPosition(TransformComponent*& transform, const std::vector<float> &information, int& index)
{
	transform->rect.x = information[index++];
	transform->rect.y = information[index++];
}
void CreationSystem::SetupSize(TransformComponent*& transform, const std::vector<float> &information, int& index)
{
	if (information[index] != -1)
	{
		transform->rect.w = information[index];
	}
	index++;

	if (information[index] != -1)
	{
		transform->rect.h = information[index];
	}
	index++;
}
void CreationSystem::SetupAngle(TransformComponent*& transform, const std::vector<float> &information, int& index)
{
	transform->angle = information[index++];
}
void CreationSystem::SetupVelocity(PhysicsComponent*& physics,const std::vector<float> &information, int& index)
{
	physics->xVelocity = information[index++];
	physics->yVelocity = information[index++];
}

void CreationSystem::EntityToSystemAssigned()
{
	_systemCreatedEntities.pop_back();
}

bool CreationSystem::Empty()
{
	return _systemCreatedEntities.empty();
}

std::pair<std::vector<SystemType>, Entity*> CreationSystem::GetSystemCreatedEntity() const
{
	return _systemCreatedEntities.back();
}

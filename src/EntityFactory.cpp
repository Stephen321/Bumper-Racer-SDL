
#include "EntityFactory.h"

#include "ColliderComponent.h"
#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "SpriteComponent.h"
#include "GunComponent.h"
#include "AIComponent.h"
#include "DestructionComponent.h"
#include "FlagComponent.h"
#include "CheckpointComponent.h"
#include "StatusEffectComponent.h"
#include "PowerUpComponent.h"

#include "ConstHolder.h"




EntityFactory::EntityFactory()
{
}

EntityFactory::~EntityFactory()
{

}


void EntityFactory::Initialize(std::map<TextureID, SDL_Texture*>* th)
{
	_textureHolder = th;
}

Entity* EntityFactory::CreateEntity(EntityType t, int id)
{
	Entity* entity = nullptr;

	switch (t)
	{
	case EntityType::Tile:
		entity = CreateTileEntity(id);
		break;
	case EntityType::Checkpoint:
		entity = CreateCheckpointEntity(id);
		break;
	case EntityType::Bullet:
		entity = CreateBulletEntity(id);
		break;
	case EntityType::PowerUp:
		entity = CreatePowerUpEntity(id);
		break;
	case EntityType::AI:
		entity = CreateAIEntity(id);
		break;
	case EntityType::Player:
		entity = CreatePlayerEntity(id);
		break;
	case EntityType::Weapon:
		entity = CreateWeaponEntity(id);
		break;
	case EntityType::Flag:
		entity = CreateFlagEntity(id);
		break;
	case EntityType::UI:
		entity = CreateUIEntity(id);
		break;

	default:
		break;
	}

	return entity;
}

Entity* EntityFactory::CreatePlayerEntity(int id)
{
	Entity* player = new Entity(EntityType::Player);
	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::Player]);

	player->AddComponent(spriteComponent);
	player->AddComponent(new TransformComponent(0, 0, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h));
	player->AddComponent(new HealthComponent(100, 100, true));
	player->AddComponent(new PhysicsComponent(0, 0, PLAYER_ACCEL_RATE, PLAYER_ACCEL_RATE, MAX_PLAYER_VELOCITY));
	player->AddComponent(new ColliderComponent());
	player->AddComponent(new FlagComponent());
	player->AddComponent(new StatusEffectComponent());

	return player;
}
Entity* EntityFactory::CreateAIEntity(int id)
{
	Entity* ai = new Entity(EntityType::AI);
	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::Player]);

	ai->AddComponent(spriteComponent);
	ai->AddComponent(new TransformComponent(0, 0, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h));
	ai->AddComponent(new HealthComponent(100, 100, true));
	ai->AddComponent(new PhysicsComponent(0, 0, AI_ACCEL_RATE, AI_ACCEL_RATE, MAX_AI_VELOCITY));
	ai->AddComponent(new ColliderComponent());
	ai->AddComponent(new FlagComponent());
	ai->AddComponent(new AIComponent());
	ai->AddComponent(new StatusEffectComponent());

	return ai;
}
Entity* EntityFactory::CreatePowerUpEntity(int id)
{
	Entity* powerUp = new Entity(EntityType::PowerUp);
	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::EntitySpriteSheet]);

	switch (id)
	{
	case 1:
	{
		spriteComponent->sourceRect = { 0, 0, 0, 0 };
		break;
	}
	case 2:
	{
		spriteComponent->sourceRect = { 1, 0, 0, 0 };
		break;
	}
	case 3:
	{
		spriteComponent->sourceRect = { 2, 0, 0, 0 };
		break;
	}
	default:
		break;
	}

	powerUp->AddComponent(spriteComponent);
	powerUp->AddComponent(new TransformComponent(0, 0, 48, 48));
	powerUp->AddComponent(new ColliderComponent());
	powerUp->AddComponent(new DestructionComponent());
	powerUp->AddComponent(new PowerUpComponent(static_cast<PowerUpComponent::Type>(id)));

	return powerUp;
}
Entity* EntityFactory::CreateWeaponEntity(int id)
{
	Entity* weapon = new Entity(EntityType::Weapon);

	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::Weapon]);

	weapon->AddComponent(new TransformComponent(0.f, 0.f, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h, spriteComponent->sourceRect.w*0.25f, spriteComponent->sourceRect.h*0.5f, 1.0f, 1.0f, 0));
	weapon->AddComponent(spriteComponent);
	weapon->AddComponent(new GunComponent(BULLET_FIRE_RATE, BULLET_AMMO, id));
	weapon->AddComponent(new DestructionComponent());

	return weapon;
}
Entity* EntityFactory::CreateBulletEntity(int id)
{
	Entity* bullet = new Entity(EntityType::Bullet);

	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::Bullet]);

	bullet->AddComponent(new TransformComponent(0.f, 0.f, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h, spriteComponent->sourceRect.w*0.5f, spriteComponent->sourceRect.h*0.5f, 1.0f, 1.0f, 0));
	bullet->AddComponent(spriteComponent);
	bullet->AddComponent(new PhysicsComponent(0, 0, 0, 0, MAX_BULLET_VELOCITY));
	bullet->AddComponent(new ColliderComponent());
	bullet->AddComponent(new DestructionComponent());

	return bullet;
}
Entity* EntityFactory::CreateCheckpointEntity(int id)
{
	Entity* checkpoint = new Entity(EntityType::Checkpoint);

	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::Checkpoint]);

	checkpoint->AddComponent(spriteComponent);
	checkpoint->AddComponent(new TransformComponent(0.f, 0.f, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h, spriteComponent->sourceRect.w * 0.5f, spriteComponent->sourceRect.h*0.5f, 1.0f, 1.0f, 0));
	checkpoint->AddComponent(new ColliderComponent());
	checkpoint->AddComponent(new CheckpointComponent(id));

	return checkpoint;
}
Entity* EntityFactory::CreateFlagEntity(int id)
{
	Entity* flag = new Entity(EntityType::Flag);

	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::Flag]);

	flag->AddComponent(spriteComponent);
	flag->AddComponent(new TransformComponent(0.f, 0.f, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h, spriteComponent->sourceRect.w*0.5f, spriteComponent->sourceRect.h*0.5f, 1.0f, 1.0f, 0));
	flag->AddComponent(new ColliderComponent());
	flag->AddComponent(new PhysicsComponent(0, 0, PLAYER_ACCEL_RATE, PLAYER_ACCEL_RATE, MAX_PLAYER_VELOCITY));

	return flag;
}
Entity* EntityFactory::CreateTileEntity(int id)
{
	Entity* tile = new Entity(EntityType::Tile);

	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::TilemapSpriteSheet]);

	switch (id)
	{
	case 1:
	{
		spriteComponent->sourceRect = { 0, 0, 0, 0 };
		break;
	}
	case 2:
	{
		spriteComponent->sourceRect = { 1, 0, 0, 0 };
		break;
	}
	case 3:
	{
		spriteComponent->sourceRect = { 2, 0, 0, 0 };
		break;
	}
	case 4:
	{
		spriteComponent->sourceRect = { 3, 0, 0, 0 };
		break;
	}
	case 5:
	{
		spriteComponent->sourceRect = { 4, 0, 0, 0 };
		break;
	}
	case 6:
	{
		spriteComponent->sourceRect = { 0, 1, 0, 0 };
		break;
	}
	case 7:
	{
		spriteComponent->sourceRect = { 1, 1, 0, 0 };
		break;
	}
	case 8:
	{
		spriteComponent->sourceRect = { 2, 1, 0, 0 };
		break;
	}
	case 9:
	{
		spriteComponent->sourceRect = { 3, 1, 0, 0 };
		break;
	}
	case 10:
	{
		spriteComponent->sourceRect = { 4, 1, 0, 0 };
		break;
	}
	case 11:
	{
		spriteComponent->sourceRect = { 0, 2, 0, 0 };
		break;
	}
	case 12:
	{
		spriteComponent->sourceRect = { 1, 2, 0, 0 };
		break;
	}
	case 13:
	{
		spriteComponent->sourceRect = { 2, 2, 0, 0 };
		break;
	}
	case 14:
	{
		spriteComponent->sourceRect = { 3, 2, 0, 0 };
		break;
	}
	case 15:
	{
		spriteComponent->sourceRect = { 4, 2, 0, 0 };
		break;
	}
	default:
		break;
	}

	tile->AddComponent(spriteComponent);
	tile->AddComponent(new TransformComponent(0, 0, 0, 0));

	return tile;
}
Entity* EntityFactory::CreateUIEntity(int id)
{
	Entity* ui = new Entity(EntityType::UI);
	ui->AddComponent(new TransformComponent(0, 0, 0, 0));
	ui->AddComponent(new SpriteComponent((*_textureHolder)[TextureID::EntitySpriteSheet]));

	return ui;
}
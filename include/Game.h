#ifndef GAME_H
#define GAME_H

#include "Debug.h"
#include "MyContactListener.h"

#include "SDL_image.h"
#include "Box2D\Box2D.h"

#include "Entity.h"
#include "SpriteComponent.h"
#include "BoundsComponent.h"

#include "RenderSystem.h"

#include <SDL.h>
#include <vector>
#include <queue>
#include <map>

enum class TextureID
{
	Player,
};

class Game
{
public:
									Game();
									~Game();

	bool							Initialize(const char* title, int xpos, int ypos, int width, int height, int flags);
	
	void							Render();
	void							Update();
	void							LoadContent();
	void							HandleEvents();
	void							CleanUp();

	bool							IsRunning();

private:
	bool							SetupSDL(const char* title, int xpos, int ypos, int width, int height, int flags);

private:
	SDL_Window*						_window;
	SDL_Renderer*					_renderer;

	std::map<TextureID, SDL_Texture*>_textureHolder;

	b2Vec2							 _gravity;
	MyContactListener				 _contactListener;
	b2World							 _world;

	bool							_running;

	unsigned int					_lastTime;//time of last update;


	std::vector<Entity*>			_entities;
	RenderSystem					_renderSystem;
	
};



#endif


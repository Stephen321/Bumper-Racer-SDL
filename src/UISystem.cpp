#include "UISystem.h"

#include "TransformComponent.h"
#include "SpriteComponent.h"

UISystem::UISystem(float updateRate)
	: System(updateRate)
{
}


UISystem::~UISystem()
{
}

void UISystem::Initialize(SDL_Renderer* renderer)
{
	_renderer = renderer;

	// Text
	TTF_Init();
	_fontSize = 32;
	_font = TTF_OpenFont("Media\\Fonts\\font.ttf", _fontSize);
	_textTexture = std::vector<SDL_Texture*>();
	_textRectangle = std::vector<SDL_Rect>();

	//TTF_CloseFont(_font); // Free Font Memory
}

void UISystem::Process(float dt)
{
	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;
		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* e : (*it).second)
			{
				// Transform to screen
				TransformComponent* transform = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));
				SpriteComponent* sprite = static_cast<SpriteComponent*>(e->GetComponent(Component::Type::Sprite));

				SDL_Rect scaledRect;
				if (transform != nullptr)
				{
					scaledRect = transform->rect;
					scaledRect.w *= transform->scaleX;
					scaledRect.h *= transform->scaleY;
					scaledRect.x -= transform->origin.x * transform->scaleX;
					scaledRect.y -= transform->origin.y * transform->scaleY;
				}

				if (transform != nullptr && sprite != nullptr)
				{
					//Render to screen
					SDL_RenderCopyEx(_renderer, sprite->texture, &sprite->sourceRect, &scaledRect, 
						transform->angle, &transform->origin, SDL_FLIP_NONE);
				}

				// Draw Text
				for (int i = 0; i < _textTexture.size(); i++)
				{
					SDL_RenderCopy(_renderer, _textTexture[i], NULL, &_textRectangle[i]);
				}
			}
		}
	}
}

SDL_Texture* UISystem::CreateText(std::string message, int x, int y)
{
	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ 255, 255, 255, 255 });

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, surface);
	_textTexture.push_back(textTexture);

	int width, height;
	SDL_QueryTexture(_textTexture.back(), NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = x;
	textRectangle.y = y;
	textRectangle.w = width;
	textRectangle.h = height;
	_textRectangle.push_back(textRectangle);

	SDL_FreeSurface(surface);
	SDL_RenderCopy(_renderer, _textTexture.back(), NULL, &_textRectangle.back());

	return _textTexture.back();
}

void UISystem::CreateTextColoured(std::string message, int x, int y, Uint8 r, Uint8 b, Uint8 g, Uint8 a)
{
	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ r, g, b, a });

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, surface);
	_textTexture.push_back(textTexture);

	int width, height;
	SDL_QueryTexture(_textTexture.back(), NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = x;
	textRectangle.y = y;
	textRectangle.w = width;
	textRectangle.h = height;
	_textRectangle.push_back(textRectangle);

	SDL_FreeSurface(surface);
	SDL_RenderCopy(_renderer, _textTexture.back(), NULL, &_textRectangle.back());
}

void UISystem::UpdateText(std::string message, int index)
{
	// Destroy Previous Image
	SDL_DestroyTexture(_textTexture[index]);

	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ 255, 255, 255, 255 });

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, surface);
	_textTexture[index] = textTexture;

	int width, height;
	SDL_QueryTexture(_textTexture[index], NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = _textRectangle[index].x;
	textRectangle.y = _textRectangle[index].y;
	textRectangle.w = width;
	textRectangle.h = height;
	_textRectangle[index] = textRectangle;

	SDL_FreeSurface(surface);
	SDL_RenderCopy(_renderer, _textTexture[index], NULL, &_textRectangle[index]);
}

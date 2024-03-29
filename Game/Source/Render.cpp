#include "App.h"
#include "Window.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

#define VSYNC true

Render::Render() : Module()
{
	name.Create("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
Render::~Render()
{}

// Called before render is available
bool Render::Awake(pugi::xml_node config)
{
	LOG("Create SDL rendering context");
	bool ret = true;

	Uint32 flags = SDL_RENDERER_ACCELERATED;

	//Load the VSYNC status from config.xml and adapt the code to set it on / off
	if (config.child("vsync").attribute("value").as_bool()) {
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}
	else {
		LOG("vsync OFF");
	}
	 
	renderer = SDL_CreateRenderer(app->win->window, -1, flags);

	if(renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera.w = app->win->screenSurface->w;
		camera.h = app->win->screenSurface->h;
		camera.x = 0;
		camera.y = 0;
	}

	//New mouse
	mouseText = app->tex->Load(config.child("mouse").attribute("texturePath").as_string());
	SDL_ShowCursor(0);

	//Font initializtation
	TTF_Init();
	font = TTF_OpenFont(config.child("font").attribute("arialPath").as_string(), 200);

	return ret;
}

// Called before the first frame
bool Render::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);
	return true;
}

// Called each loop iteration
bool Render::PreUpdate()
{
	SDL_RenderClear(renderer);
	return true;
}

bool Render::Update(float dt)
{
	if (app->debug && app->map->active)
	{
		int X, Y;
		app->input->GetMousePosition(X, Y);
		X -= app->render->camera.x;
		Y -= app->render->camera.y;
		//Mouse pixel pos print
		mousePos = "Mouse Pos: (" + std::to_string(X) + ", " + std::to_string(Y) + ")";
		const char* text = mousePos.c_str();
		DrawText(text, 0, 10, 250, 30, 0, 0, 255);

		// Get the mouse position and obtain the map coordinate
		X -= app->map->GetTileWidth() / 2;
		Y -= app->map->GetTileHeight() / 2;
		iPoint mouseTile = app->map->WorldToMap(X ,	Y);

		// Render a texture where the mouse is over to highlight the tile, use the texture 'mouseTileTex'
		iPoint highlightedTileWorld = app->map->MapToWorld(mouseTile.x, mouseTile.y);
		app->render->DrawTexture(selectionTex, highlightedTileWorld.x, highlightedTileWorld.y + app->map->GetTileHeight() / 2);

		//Mouse Tile print text
		mousePos = "Mouse Tile : (" + std::to_string(app->map->WorldToMap(X, Y).x) + ", " + std::to_string(app->map->WorldToMap(X, Y).y) + ")";
		text = mousePos.c_str();
		DrawText(text, 0, 50, 250, 30, 0, 0, 255);

		//Player tile print text
		X = app->sceneManager->currentScene->GetPlayer()->GetTile().x;
		Y = app->sceneManager->currentScene->GetPlayer()->GetTile().y;
		int direction = static_cast<int>(app->sceneManager->currentScene->GetPlayer()->PosState);
		playerPos = "Player Tile: (" + std::to_string(X) + ", " + std::to_string(Y) + ") Direction: " + std::to_string(direction) + ")";
		text = playerPos.c_str();
		DrawText(text, 0, 80, 250, 30, 0, 0, 255);
	}

	return true;
}

bool Render::PostUpdate()
{
	//New mouse
	int X, Y;
	app->input->GetMousePosition(X, Y);
	X -= app->render->camera.x;
	Y -= app->render->camera.y;
	DrawTexture(mouseText, X, Y, NULL);

	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

// Called before quitting
bool Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

void Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void Render::SetSelectionTex(SDL_Texture* selectionTex)
{
	if (selectionTex != nullptr) {
		app->tex->UnLoad(this->selectionTex);
	}
	this->selectionTex = selectionTex;
}

void Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

// Blit to screen
bool Render::DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, double angle, int pivotX, int pivotY) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * scale;
	rect.y = (int)(camera.y * speed) + y * scale;

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if(pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot.x = pivotX;
		pivot.y = pivotY;
		p = &pivot;
	}

	if(SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::IDrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, double angle, int pivotX, int pivotY) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * scale;
	rect.y = (int)(camera.y * speed) + y * scale;

	if (section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if (pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot.x = pivotX;
		pivot.y = pivotY;
		p = &pivot;
	}

	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_HORIZONTAL) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if (use_camera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if (use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for (uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x * scale + camera.x) + (radius * cos(i * factor));
		points[i].y = (int)(y * scale + camera.y) + (radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawText(const char* text, int posx, int posy, int w, int h, int r, int g, int b)
{
	SDL_Color color = { r,g,b };
	SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect = { posx, posy, w, h };

	SDL_RenderCopy(renderer, texture, NULL, &dstrect);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);

	return true;
}
#include "Credits.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"

Credits::Credits() : Scene()
{
	name.Create("credits");
}

Credits::~Credits()
{
}

bool Credits::Awake(pugi::xml_node config)
{
	sceneconfig = config;
	return true;
}

bool Credits::Start()
{
	img = app->tex->Load(sceneconfig.attribute("texturePath").as_string());
	app->tex->GetSize(img, texW, texH);
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	app->win->GetWindowSize(windowW, windowH);
	return true;
}

bool Credits::PreUpdate()
{
	return true;
}

bool Credits::Update(float dt)
{
	app->render->DrawTexture(img, windowW / 2 - texW / 2, windowH / 2 - texH / 2, NULL);
	return true;
}

bool Credits::PostUpdate()
{

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP || app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		app->sceneManager->ChangeScane((Scene*)app->sceneManager->menu);

	return true;
}

bool Credits::CleanUp()
{
	app->tex->UnLoad(img);
	return true;
}

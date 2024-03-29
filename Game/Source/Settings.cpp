#include "Settings.h"
#include "Window.h"
#include "Input.h"
#include "SceneManager.h"
#include "GuiManager.h"
#include "Audio.h"

Settings::Settings() : Scene()
{
	name.Create("settings");
}

Settings::~Settings()
{
}

bool Settings::Awake(pugi::xml_node config)
{
	return true;
}

bool Settings::Start()
{
	app->win->GetWindowSize(windowW, windowH);

	quat.w = windowW;
	quat.h = windowH;
	quat.x = 0;
	quat.y = 0;

	SDL_Rect sliderMusic = { windowW / 2 - 200, windowH / 2 - 10 - (windowH / 10) * 2, 400,20 };
	SDL_Rect sliderMusicBounds = { sliderMusic.x + sliderMusic.w - 15, sliderMusic.y + (sliderMusic.h / 2) - 15, 30, 30 };
	gcMSlider = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 6, "MUSIC VOLUME", sliderMusic, this, sliderMusicBounds);

	SDL_Rect sliderPos = { windowW / 2 - 200,  windowH / 2 - 10 - (windowH / 20), 400,20 };
	SDL_Rect sliderBoundsPos = { sliderPos.x + sliderPos.w - 15, sliderPos.y + (sliderPos.h / 2) - 15, 30, 30 };
	gcFSlider = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 7, "FX VOLUME", sliderPos, this, sliderBoundsPos);

	SDL_Rect checkboxFS = { windowW / 2 - 15 + 150, windowH / 2 - 15 + (windowH / 15), 50,50 };
	checkBoxFS = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 8, "FULL SCREEN", checkboxFS, this);

	SDL_Rect checkboxVS = { windowW / 2 - 15 + 85, windowH / 2 - 15 + (windowH / 10) * 2, 50,50 };
	checkBoxVS = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 9, "V-SYNC", checkboxVS, this);

	fullscreen = checkBoxFS->checked;
	
	return true;
}

bool Settings::PreUpdate()
{
	return true;
}

bool Settings::Update(float dt)
{
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(125));
	SDL_RenderFillRect(app->render->renderer, &quat);

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		app->sceneManager->CloseSettings();
	}

	return true;
}

bool Settings::PostUpdate()
{
	return true;
}

bool Settings::CleanUp()
{
	app->guiManager->DeleteGuiControl(gcMSlider);
	app->guiManager->DeleteGuiControl(gcFSlider);
	checkBoxFS->CleanUp();
	app->guiManager->DeleteGuiControl(checkBoxFS);
	checkBoxVS->CleanUp();
	app->guiManager->DeleteGuiControl(checkBoxVS);
	return true;
}

bool Settings::OnGuiMouseClickEvent(GuiControl* control)
{
	if (control->id == 8) {
		if (!fullscreen) {
			SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
			fullscreen = true;
		}
		else {
			SDL_SetWindowFullscreen(app->win->window, 0);
			SDL_SetWindowSize(app->win->window, windowW, windowH);
			fullscreen = false;
		}
	}
	if (control->id == 6) {
		app->audio->ChangeMusicVolume(gcMSlider->percent);
	}
	if (control->id == 7) {
		app->audio->ChangeFxVolume(gcFSlider->percent);
	}
	if (control->id == 9) {
		if (app->maxFrameDuration == 16) {
			app->maxFrameDuration = 32;
		}
		else {
			app->maxFrameDuration = 16;
		}
	}

	return true;
}

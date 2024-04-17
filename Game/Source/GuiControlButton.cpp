#include "GuiControlButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "GuiManager.h"

GuiControlButton::GuiControlButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;
}

GuiControlButton::~GuiControlButton()
{

}

bool GuiControlButton::Update(float dt)
{
	//debug = app->guiManager->debug;

	if (state != GuiControlState::DISABLED)
	{
		//Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		//If the position of the mouse if inside the bounds of the button 
		if (mouseX > bounds.x && mouseX < bounds.x + bounds.w && mouseY > bounds.y && mouseY < bounds.y + bounds.h) {
		
			state = GuiControlState::FOCUSED;

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
				state = GuiControlState::PRESSED;
			}
			
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
				NotifyObserver();
				app->audio->PlayFx(app->guiManager->butonFx);
			}
		}
		else {
			state = GuiControlState::NORMAL;
		}

		//Draw the button according the GuiControl State
		switch (state)
		{
		case GuiControlState::DISABLED:

			break;
		case GuiControlState::NORMAL:
			if (debug) {
				app->render->DrawRectangle(bounds, 239, 184, 16, 255, true, false);
			}
			app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h, 0, 0, 0);
			anim = false;
			animW = 0;
			animH = 0;
			break;
		case GuiControlState::FOCUSED:
			if (debug) {
				app->render->DrawRectangle(bounds, 239, 184, 16, 255, true, false);
			}
			app->render->DrawText(text.GetString(), bounds.x - (int)animW, bounds.y - (int)animH, bounds.w + (int)animW*2, bounds.h + (int)animH*2, 255,255,255);
			if (animated) {
				if (!anim) {
					animW += animSpeed * dt;
					animH += animSpeed * dt;
					if (animW >= 10) {
						anim = !anim;
					}
				}
				else {
					animW -= animSpeed * dt;
					animH -= animSpeed * dt;
					if (animW <= 0) {
						anim = !anim;
					}
				}
			}
			break;
		case GuiControlState::PRESSED:
			if (debug) {
				app->render->DrawRectangle(bounds, 239, 184, 16, 255, true, false);
			}
			app->render->DrawText(text.GetString(), bounds.x + 5, bounds.y + 5, bounds.w - 10, bounds.h - 10, 125, 125, 125);
			break;
		}

	}
	else {
		if (debug)
		app->render->DrawRectangle(bounds, 239, 184, 16, 255, true, false);
		app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h, 50, 50, 50);
	}

	return false;
}


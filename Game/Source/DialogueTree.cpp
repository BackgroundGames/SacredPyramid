#include "DialogueTree.h"

#include "App.h"
#include "Render.h"
#include "GuiManager.h"
#include "GuiControlButton.h"
#include "SceneManager.h"
#include "Window.h"
#include "Log.h"
#include "Textures.h"

#include "SceneManager.h"
#include "Player.h"

#include <cstring>
#include <algorithm>

DialogueTree::DialogueTree() : Module()
{
	name.Create("dialogues");
}

DialogueOption::DialogueOption(const char* Text, int ReturnCode, DialogueNode* NextNode)
{
	text = Text;
	returnCode = ReturnCode;
	nextNode = NextNode;
}

DialogueNode::DialogueNode(const char* Text)
{
	text = Text;
}

DialogueTree::~DialogueTree()
{

}

bool DialogueTree::Awake(pugi::xml_node config)
{
	active = false;
	treeConf = config;

	return true;
}

bool DialogueTree::Start()
{
	app->win->GetWindowSize(windowW, windowH);

	quat.w = windowW/2;
	quat.h = windowH/4;
	quat.x = (windowW/2) - (quat.w/2);
	quat.y = (windowH / 2) + (quat.h / 2);

	textures.push_back(app->tex->Load(treeConf.attribute("zhaak").as_string()));
	textures.push_back(app->tex->Load(treeConf.attribute("eli").as_string()));
	textures.push_back(app->tex->Load(treeConf.attribute("bandit").as_string()));
	textures.push_back(app->tex->Load(treeConf.attribute("drunkard").as_string()));
	textures.push_back(app->tex->Load(treeConf.attribute("npc1").as_string()));
	textures.push_back(app->tex->Load(treeConf.attribute("bartender").as_string()));

	return true;
}

bool DialogueTree::Update(float dt)
{
	if (delOptions) {
		deleteOptions();
	}

	uint textW, textH;

	app->tex->GetSize(currentNode->texture, textW, textH);
	app->render->DrawTexture(currentNode->texture, (windowW/2) - app->render->camera.x - (textW / 2), (windowH/2) - (textH/3) - app->render->camera.y + 45, NULL);

	//SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(125));
	//SDL_RenderFillRect(app->render->renderer, &quat);

	longitud_total = std::strlen(currentNode->text);

	// Definir la longitud de las secciones
	longitud_seccion = quat.w/12; // Por ejemplo, longitud de cada sección

	// Calcular el número total de secciones
	numero_secciones = longitud_total / longitud_seccion;
	if (longitud_total % longitud_seccion != 0) {
		numero_secciones++; // Asegurar que la última sección contenga el resto de caracteres
	}

	// Iterar sobre cada sección e imprimirla
	for (int i = 0; i < numero_secciones; ++i) {
		int indice_inicial = i * longitud_seccion;

		// Calcular la longitud de la sección actual
		int longitud_actual = std::min(longitud_seccion, longitud_total - indice_inicial);

		// Crear un buffer para almacenar la sección actual
		char* seccion_actual = new char[longitud_actual + 1]; // +1 para el carácter nulo terminador

		// Copiar la sección actual del texto original al buffer
		for (int j = 0; j < longitud_actual; ++j) {
			seccion_actual[j] = currentNode->text[indice_inicial + j];
		}
		seccion_actual[longitud_actual] = '\0'; // Asegurarse de que el buffer está terminado con el carácter nulo


		// Imprimir la sección actual
		app->render->DrawText(seccion_actual, quat.x + 50, quat.y + (20 * i) + 100, longitud_actual * 10, 20, 0, 0, 0);

		// Liberar la memoria asignada para la sección actual
		delete[] seccion_actual;
	}

	return true;
}

bool DialogueTree::CleanUp()
{
	for (int i = 0; i < optionNodes.size(); i++) {
		app->guiManager->DeleteGuiControl((GuiControl*)optionNodes.at(i));
		delete optionNodes[i];
	}

	optionNodes.clear();

	for (int i = 0; i < dialogueNodes.size(); i++) {
		delete dialogueNodes[i];
	}

	dialogueNodes.clear();

	if (active) {
		app->guiManager->CleanUp();
	}

	for (size_t i = 0; i < app->sceneManager->currentScene->players.size(); i++)
	{
		dynamic_cast<Player*>(app->sceneManager->currentScene->players.at(i))->exploringState = ExploringState::IDLE;
		dynamic_cast<Player*>(app->sceneManager->currentScene->players.at(i))->interacted = nullptr;
	}

	active = false;

	if (dialog == "dialogue4" && app->sceneManager->currentScene != (Scene*)app->sceneManager->menu) {
		app->sceneManager->currentScene->GetPlayer()->exploringState = ExploringState::NONE;
		app->sceneManager->ChangeScane((Scene*)app->sceneManager->level1);
	}

	if (dialog == "dialogue5" && app->sceneManager->currentScene != (Scene*)app->sceneManager->menu) {
		app->sceneManager->currentScene->GetPlayer()->exploringState = ExploringState::NONE;
		app->sceneManager->ChangeScane((Scene*)app->sceneManager->level4);
	}

	return true;
}

int DialogueTree::performDialogue(const char * dialogueName)
{
	dialog = dialogueName;

	pugi::xml_node itemNode = treeConf.child(dialogueName).child("node");

	int aux = 0;

	for (itemNode = itemNode; itemNode; itemNode = itemNode.next_sibling("node"))
	{
		DialogueNode* node = new DialogueNode(itemNode.attribute("text").as_string());
		longitud_total = std::strlen(node->text);

		// Definir la longitud de las secciones
		longitud_seccion = quat.w / 8 - 1; // Por ejemplo, longitud de cada sección

		// Calcular el número total de secciones
		numero_secciones = longitud_total / longitud_seccion;
		if (longitud_total % longitud_seccion != 0) {
			numero_secciones++; // Asegurar que la última sección contenga el resto de caracteres
		}

		// Iterar sobre cada sección e imprimirla
		for (int i = 0; i < numero_secciones; ++i) {
			int indice_inicial = i * longitud_seccion;

			// Calcular la longitud de la sección actual
			int longitud_actual = std::min(longitud_seccion, longitud_total - indice_inicial);

			// Crear un buffer para almacenar la sección actual
			char * seccion_actual = new char[longitud_actual + 1]; // +1 para el carácter nulo terminador

			// Copiar la sección actual del texto original al buffer
			for (int j = 0; j < longitud_actual; ++j) {
				seccion_actual[j] = node->text[indice_inicial + j];
			}
			seccion_actual[longitud_actual] = '\0'; // Asegurarse de que el buffer está terminado con el carácter nulo

			node->textLines[i] = seccion_actual;

			delete[] seccion_actual;
		}

		aux++;

		node->texture = textures[itemNode.attribute("speaker").as_int()];

		dialogueNodes.push_back(node);
	}

	itemNode = treeConf.child(dialogueName).child("node");

	for (size_t i = 0; i < dialogueNodes.size(); i++)
	{
		for (pugi::xml_node optionNode = itemNode.child("option"); optionNode; optionNode = optionNode.next_sibling("option"))
		{
			if (optionNode.attribute("pnode").as_int() == -1) {
				dialogueNodes[i]->dialogueOptions.push_back(DialogueOption(optionNode.attribute("text").as_string(),
					optionNode.attribute("returnCode").as_int(), nullptr));
			}
			else {
				dialogueNodes[i]->dialogueOptions.push_back(DialogueOption(optionNode.attribute("text").as_string(),
					optionNode.attribute("returnCode").as_int(), dialogueNodes[optionNode.attribute("pnode").as_int()]));
			}
		}
		itemNode = itemNode.next_sibling("node");
	}

	if (dialogueNodes.empty()) 
	{
		return -1;
	}

	currentNode = dialogueNodes[0];

	for (int i = 0; i < currentNode->dialogueOptions.size(); i++) {
		size_t Size = strlen(currentNode->dialogueOptions[i].text);
		SDL_Rect playPos = { quat.x + 40, quat.y + (20 * i) + 150, 10 * Size,20 };
		optionNodes.push_back((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, i, currentNode->dialogueOptions[i].text, playPos, app->sceneManager->currentScene));
		optionNodes[i]->animated = false;
	}

	active = true;

	return 0;
}

bool DialogueTree::ChoseOption(int optionid)
{

	delOptions = true;

	currentNode = currentNode->dialogueOptions[optionid].nextNode;

	if (currentNode == nullptr) {
		CleanUp();
		return true;
	}

	app->input->ResetMouseButtonState();

	return true;
}

void DialogueTree::deleteOptions()
{
	for (int i = 0; i < optionNodes.size(); i++) {
		app->guiManager->DeleteGuiControl((GuiControl*)optionNodes.at(i));
		delete optionNodes[i];
	}

	optionNodes.clear();

	delOptions = false;

	for (int i = 0; i < currentNode->dialogueOptions.size(); i++) {
		size_t Size = strlen(currentNode->dialogueOptions[i].text);
		SDL_Rect playPos = { quat.x + 40, quat.y + (20 * i) + 150, 10 * Size,20 };
		optionNodes.push_back((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, i, currentNode->dialogueOptions[i].text, playPos, app->sceneManager->currentScene));
		optionNodes[i]->animated = false;
	}
	
}

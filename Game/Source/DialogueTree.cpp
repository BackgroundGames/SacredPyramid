#include "DialogueTree.h"

#include "App.h"
#include "Render.h"
#include "GuiManager.h"
#include "SceneManager.h"
#include "Log.h"

DialogueTree::DialogueTree() : Module()
{
	name.Create("dialoguetree");
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

	return true;
}

bool DialogueTree::Start()
{
	return true;
}

bool DialogueTree::Update(float dt)
{
	size_t Size = strlen(currentNode->text);

	app->render->DrawText(currentNode->text, 0, 0, Size * 25, 50, 0, 0, 255);

	return true;
}

bool DialogueTree::CleanUp()
{
	active = false;

	for (int i = 0; i < dialogueNodes.size(); i++) {
		delete dialogueNodes[i];
	}

	dialogueNodes.clear();

	app->guiManager->CleanUp();

	return true;
}

int DialogueTree::performDialogue()
{

	DialogueNode* node0 = new DialogueNode("hola que tal");
	DialogueNode* node1 = new DialogueNode("llastima");
	DialogueNode* node2 = new DialogueNode("si? que guai, 4 o 5?");
	DialogueNode* node3 = new DialogueNode("nem al 5?");
	DialogueNode* node4 = new DialogueNode("soc 5  i final");

	node0->dialogueOptions.push_back(DialogueOption("mal", 0, node1));
	node0->dialogueOptions.push_back(DialogueOption("be", 0, node2));
	dialogueNodes.push_back(node0);

	node1->dialogueOptions.push_back(DialogueOption("chao", 0, nullptr));
	dialogueNodes.push_back(node1);

	node2->dialogueOptions.push_back(DialogueOption("chao", 0, nullptr));
	node2->dialogueOptions.push_back(DialogueOption("5", 0, node4));
	node2->dialogueOptions.push_back(DialogueOption("4", 0, node3));
	dialogueNodes.push_back(node2);

	node3->dialogueOptions.push_back(DialogueOption("go 5", 0, node4));
	node3->dialogueOptions.push_back(DialogueOption("im out", 0, nullptr));
	dialogueNodes.push_back(node3);

	node4->dialogueOptions.push_back(DialogueOption("acepto", 1, nullptr));
	node4->dialogueOptions.push_back(DialogueOption("im out", 0, nullptr));
	dialogueNodes.push_back(node4);

	if (dialogueNodes.empty()) {
		return -1;
	}

	currentNode = dialogueNodes[0];

	for (int i = 0; i < currentNode->dialogueOptions.size(); i++) {
		size_t Size = strlen(currentNode->dialogueOptions[i].text);
		SDL_Rect playPos = { 0,  50 + 50 * i, 25 * Size,50 };
		optionNodes.push_back((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, i, currentNode->dialogueOptions[i].text, playPos, app->sceneManager->currentScene));
	}

	active = true;

	return 0;
}

bool DialogueTree::ChoseOption(int optionid)
{

	deleteOptions();

	currentNode = currentNode->dialogueOptions[optionid].nextNode;

	if (currentNode == nullptr) {
		CleanUp();
		return true;
	}

	for (int i = 0; i < currentNode->dialogueOptions.size(); i++) {
		size_t Size = strlen(currentNode->dialogueOptions[i].text);
		SDL_Rect playPos = { 0,  50 + 50 * i, 25 * Size,50 };
		optionNodes.push_back((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, i, currentNode->dialogueOptions[i].text, playPos, app->sceneManager->currentScene));
	}

	return true;
}

void DialogueTree::deleteOptions()
{
	for (int i = 0; i < optionNodes.size(); i++) {
		app->guiManager->DeleteGuiControl((GuiControl*)optionNodes.at(i));
	}

	optionNodes.clear();
	
}

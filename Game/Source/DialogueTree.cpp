#include "DialogueTree.h"

#include "App.h"
#include "Render.h"
#include "GuiManager.h"
#include "SceneManager.h"
#include "Log.h"

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

int DialogueTree::performDialogue(const char * dialogueName)
{
	pugi::xml_node itemNode = treeConf.child(dialogueName).child("node");

	for (itemNode = itemNode; itemNode; itemNode = itemNode.next_sibling("node"))
	{
		DialogueNode* node = new DialogueNode(itemNode.attribute("text").as_string());
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

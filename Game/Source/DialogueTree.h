#ifndef __DIALOGUETREE_H__
#define __DIALOGUETREE_H__

#include "Module.h"
#include <vector>

#include "PugiXml/src/pugixml.hpp"

using namespace std;

class DialogueNode;

class DialogueOption {
public:
	DialogueOption(const char* Text, int ReturnCode, DialogueNode* NextNode);
	const char* text;
	int returnCode;
	DialogueNode* nextNode;
};

class DialogueNode {
public:
	DialogueNode(const char* Text);
	const char* text;
	vector <DialogueOption> dialogueOptions;
};

class GuiControlButton;

class DialogueTree :  public Module
{
public:

	DialogueTree();

	~DialogueTree();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	int performDialogue(const char* dialogueName);

	bool ChoseOption(int optionid);

	void deleteOptions();

private:
	vector <DialogueNode*> dialogueNodes;
	vector <GuiControlButton*> optionNodes;
	DialogueNode* currentNode;
	pugi::xml_node treeConf;
};

#endif
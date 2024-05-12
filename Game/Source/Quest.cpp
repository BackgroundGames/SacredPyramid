#include "Quest.h"

Quest::Quest() {
}

Quest::~Quest()
{
}

bool Quest::Awake(pugi::xml_node config)
{
	return false;
}

//Talk To

TalkTo::TalkTo(int id) : Quest() {
	if (id == 0) {
		questName = "Seek for help";
		
	}
	else if (id == 1) {
		questName = "Go to the bar";
	}
	else if (id == 2) {
		questName = "Enter the pyramid";
	}


	questDescription = "incompleted";
	this->id = id;
}

bool TalkTo::Start()
{
	return true;
}

bool TalkTo::Update(float dt)
{
	return true;
}

bool TalkTo::CleanUp()
{
	return true;
}

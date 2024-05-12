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

TalkTo::TalkTo() : Quest() {
	questName = "hola";
}

bool TalkTo::Start()
{
	return true;
}

bool TalkTo::Update(float dt)
{
	if (completed) {
		questName = "done";
	}
	return true;
}

bool TalkTo::CleanUp()
{
	return true;
}

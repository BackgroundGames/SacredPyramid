#include "Eli.h"
#include "App.h"
#include "Audio.h"

Eli::Eli()
{
	name.Create("eli");
	subtype = PlayerType::ELI;
	maxHealth = 3;

	Awake();
}

Eli::Eli(Stats stats, Inventory inventory)
{
	name.Create("eli");
	this->stats = stats;
	this->inventory = inventory;
	subtype = PlayerType::ELI;
	Awake();
}

Eli::~Eli() {}

bool Eli::Awake()
{
	Character::Awake();

	pugi::xml_node auxAnim = parametersAnim.child("idleAnim");

	idleAnim.speed = auxAnim.attribute("speed").as_float();
	idleAnim.loop = auxAnim.attribute("loop").as_bool();

	for (pugi::xml_node idleNode = auxAnim.child("idle"); idleNode; idleNode = idleNode.next_sibling("idle"))
	{
		idleAnim.PushBack({ idleNode.attribute("x").as_int(), idleNode.attribute("y").as_int() ,idleNode.attribute("w").as_int() ,idleNode.attribute("h").as_int() });
	}

	auxAnim = parametersAnim.child("idleBAnim");

	idleAnimB.speed = auxAnim.attribute("speed").as_float();
	idleAnimB.loop = auxAnim.attribute("loop").as_bool();

	for (pugi::xml_node idleBNode = auxAnim.child("idleB"); idleBNode; idleBNode = idleBNode.next_sibling("idleB"))
	{
		idleAnimB.PushBack({ idleBNode.attribute("x").as_int(), idleBNode.attribute("y").as_int() ,idleBNode.attribute("w").as_int() ,idleBNode.attribute("h").as_int() });
	}

	atack = app->audio->LoadFx("Assets/Audio/Fx/BookSlap.wav");

	return true;
}

bool Eli::Start()
{
	Player::Start();
	return true;
}

bool Eli::Update(float dt)
{	
	Player::Update(dt);
	return true;
}

bool Eli::CleanUp()
{
	Player::CleanUp();
	return true;
}

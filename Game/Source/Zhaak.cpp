#include "Zhaak.h"
#include "App.h"

Zhaak::Zhaak()
{
	name.Create("zhaak");
	subtype = PlayerType::ZHAAK;
	inventory.weapon.damage = 1;
	inventory.weapon.range = 1;
	stats.health = 1;
	Awake();
}

Zhaak::Zhaak(Stats stats)
{
	name.Create("zhaak");
	this->stats = stats;
	subtype = PlayerType::ZHAAK;
	//porves


	Awake();
}

Zhaak::~Zhaak() {}

bool Zhaak::Awake()
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

	auxAnim = parametersAnim.child("walkingAnim");

	walkingAnim.speed = auxAnim.attribute("speed").as_float();
	walkingAnim.loop = auxAnim.attribute("loop").as_bool();

	for (pugi::xml_node idleNode = auxAnim.child("walk"); idleNode; idleNode = idleNode.next_sibling("walk"))
	{
		walkingAnim.PushBack({ idleNode.attribute("x").as_int(), idleNode.attribute("y").as_int() ,idleNode.attribute("w").as_int() ,idleNode.attribute("h").as_int() });
	}

	return true;
}

bool Zhaak::Start()
{
	Player::Start();
	return true;
}

bool Zhaak::Update(float dt)
{
	Player::Update(dt);
	return true;
}

bool Zhaak::CleanUp()
{
	Player::CleanUp();
	return true;
}

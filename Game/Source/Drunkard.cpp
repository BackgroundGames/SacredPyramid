#include "Drunkard.h"
#include "App.h"

Drunkard::Drunkard()
{
	name.Create("drunkard");
	type = EntityType::ENEMY;
	subtype = EnemyType::DRUNKARD;
	Awake();
	//prova combat
	stats.health = 1;
	inventory.weapon.range = 1;
	inventory.weapon.damage = 1;
}

Drunkard::Drunkard(Stats stats, Inventory inventory)
{
	name.Create("drunkard");
	this->stats = stats;
	this->inventory = inventory;
	type = EntityType::ENEMY;
	subtype = EnemyType::DRUNKARD;
	Awake();
}

Drunkard::~Drunkard()
{
}

bool Drunkard::Awake()
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

	return true;
}

bool Drunkard::Start()
{
	Enemy::Start();

	return true;
}

bool Drunkard::Update(float dt)
{
	Enemy::Update(dt);

	return true;
}

bool Drunkard::CleanUp()
{
	return true;
}

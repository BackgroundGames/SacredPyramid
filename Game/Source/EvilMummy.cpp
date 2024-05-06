#include "EvilMummy.h"
#include "App.h"

EvilMummy::EvilMummy()
{
	name.Create("evilMummy");
	type = EntityType::ENEMY;
	subtype = EnemyType::EVIL_MUMMY;
	Awake();
	//prova combat
	stats.health = 1;
	inventory.weapon.range = 2;
	inventory.weapon.damage = 1;
	maxHealth = 1;
}

EvilMummy::EvilMummy(Stats stats, Inventory inventory)
{
	name.Create("evilMummy");
	this->stats = stats;
	this->inventory = inventory;
	type = EntityType::ENEMY;
	subtype = EnemyType::EVIL_MUMMY;
	Awake();
}

EvilMummy::~EvilMummy()
{
}

bool EvilMummy::Awake()
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

bool EvilMummy::Start()
{
	Enemy::Start();

	return true;
}

bool EvilMummy::Update(float dt)
{
	Enemy::Update(dt);

	return true;
}

bool EvilMummy::CleanUp()
{
	return true;
}

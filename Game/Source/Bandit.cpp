#include "Bandit.h"
#include "App.h"

Bandit::Bandit()
{
	name.Create("bandit");
	type = EntityType::ENEMY;
	subtype = EnemyType::BANDIT;
	Awake();
	//prova combat
	stats.health = 2;
	inventory.weapon.range = 2;
	inventory.weapon.damage = 1;
	maxHealth = 2;
}

Bandit::Bandit(Stats stats, Inventory inventory)
{
	name.Create("bandit");
	this->stats = stats;
	this->inventory = inventory;
	type = EntityType::ENEMY;
	subtype = EnemyType::BANDIT;
	Awake();
}

Bandit::~Bandit()
{
}

bool Bandit::Awake()
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

bool Bandit::Start()
{
	Enemy::Start();
	maxHealth = 2;
	stats.health = 2;
	return true;
}

bool Bandit::Update(float dt)
{
	Enemy::Update(dt);

	return true;
}

bool Bandit::CleanUp()
{
	return true;
}

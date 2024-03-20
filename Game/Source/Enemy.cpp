#include "Enemy.h"
#include "App.h"
#include "Map.h"
#include "Render.h"
#include "Player.h"

Enemy::Enemy()
{
	name.Create("Enemy");
	type = EntityType::ENEMY;
}

Enemy::Enemy(uint hp, uint mp, uint atk, uint def, uint matk, uint mdef, uint ini)
{
	name.Create("Enemy");
	type = EntityType::ENEMY;
	this->stats.health = hp;
	this->stats.mana = mp;
	this->stats.attack = atk;
	this->stats.defense = def;
	this->stats.magicAttack = matk;
	this->stats.magicDefense = mdef;
	this->stats.initiative = ini;
}



Enemy::~Enemy() {}

bool Enemy::Awake()
{
	return true;
}

bool Enemy::Start()
{
	Character::Start();

	//aquests valors son per el pj de prova
	texW = 28;
	texH = 62;

	//Patrol
	iPoint patrol1 = app->map->WorldToMap(500, 400);
	iPoint patrol2 = app->map->WorldToMap(600, 400);

	return true;
}

bool Enemy::Update(float dt)
{
	/*if (patrol)
	{
		if (this->position == )
		{

		}
	}*/

	//if (abs(this->position.x - app->sceneManager->currentScene->GetPlayer()->position.x) < 10 &&
	//	abs(this->position.y - app->sceneManager->currentScene->GetPlayer()->position.y) < 10)
	//{
	if (iPoint(21, 10) != prevDestination)
		moveTo(iPoint(21,10));
	//}

	Character::Update(dt);

	return true;
}

bool Enemy::CleanUp()
{
	return true;
}
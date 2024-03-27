#include "Drunkard.h"
#include "App.h"

Drunkard::Drunkard()
{
	name.Create("Drunkard");
	type = EntityType::ENEMY;
	subtype = EnemyType::DRUNKARD;
}

Drunkard::~Drunkard()
{
}

bool Drunkard::Awake()
{
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

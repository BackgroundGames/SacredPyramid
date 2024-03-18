#include "Bandit.h"
#include "App.h"

Bandit::Bandit()
{
	name.Create("Bandit");
	type = EntityType::ENEMY;
	subtype = EnemyType::BANDIT;
}

Bandit::~Bandit()
{
}

bool Bandit::Awake()
{
	return true;
}

bool Bandit::Start()
{
	Enemy::Start();

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

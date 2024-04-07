#include "Eli.h"
#include "App.h"

Eli::Eli()
{
	name.Create("Eli");
	subtype = PlayerType::ELI;
}

Eli::Eli(Stats stats)
{
	name.Create("Eli");
	this->stats = stats;
	subtype = PlayerType::ELI;
}

Eli::~Eli() {}

bool Eli::Awake()
{
	Player::Awake();
	return true;
}

bool Eli::Start()
{
	Player::Start();
	idleAnim.speed = 0.05f;
	idleAnim.PushBack({ 56, 0, 56, 123 });
	idleAnimB.loop = true;
	idleAnimB.speed = 0.05f;
	idleAnimB.PushBack({ 0, 123, 56, 123 });
	idleAnimB.PushBack({ 56, 123, 56, 123 });
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

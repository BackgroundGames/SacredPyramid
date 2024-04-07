#include "Zhaak.h"
#include "App.h"

Zhaak::Zhaak()
{
	name.Create("Zhaak");
	subtype = PlayerType::ZHAAK;
}

Zhaak::Zhaak(Stats stats)
{
	name.Create("Zhaak");
	this->stats = stats;
	subtype = PlayerType::ZHAAK;
}

Zhaak::~Zhaak() {}

bool Zhaak::Awake()
{
	Player::Awake();
	return true;
}

bool Zhaak::Start()
{
	Player::Start();
	idleAnim.speed = 0.05f;
	idleAnim.PushBack({ 56, 0, 56, 123 });
	idleAnim.PushBack({ 112, 0, 56, 123 });
	idleAnim.PushBack({168, 0, 56, 123}); 
	idleAnimB.loop = true;
	idleAnimB.speed = 0.05f;
	idleAnimB.PushBack({ 0, 123, 54, 123 });
	idleAnimB.PushBack({ 54, 123, 54, 123 });
	idleAnimB.PushBack({ 108, 123, 54, 123 });
	idleAnimB.PushBack({ 162, 123, 54, 123 });
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

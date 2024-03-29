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

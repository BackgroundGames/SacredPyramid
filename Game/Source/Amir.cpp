#include "Amir.h"
#include "App.h"

Amir::Amir()
{
	name.Create("Amir");
	subtype = PlayerType::AMIR;
}

Amir::Amir(Stats stats)
{
	name.Create("Amir");
	this->stats = stats;
	subtype = PlayerType::AMIR;
}

Amir::~Amir() {}

bool Amir::Awake()
{
	Player::Awake();
	return true;
}

bool Amir::Start()
{
	Player::Start();
	return true;
}

bool Amir::Update(float dt)
{
	destination = Player::GetMouseTile(mousePos);

	Player::Update(dt);
	return true;
}

bool Amir::CleanUp()
{
	return true;
}

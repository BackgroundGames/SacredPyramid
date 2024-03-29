#include "Amir.h"

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
	return true;
}

bool Amir::Start()
{
	return true;
}

bool Amir::Update(float dt)
{
	return true;
}

bool Amir::CleanUp()
{
	return true;
}

#include "Zhaak.h"

Zhaak::Zhaak()
{
	name.Create("Zhaak");
}

Zhaak::Zhaak(Stats stats)
{
	name.Create("Zhaak");
	this->stats = stats;
}

Zhaak::~Zhaak() {}

bool Zhaak::Awake()
{
	return true;
}

bool Zhaak::Start()
{
	return true;
}

bool Zhaak::Update(float dt)
{
	return true;
}

bool Zhaak::CleanUp()
{
	return true;
}

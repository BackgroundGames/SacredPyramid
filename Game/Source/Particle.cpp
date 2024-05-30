#include "Particle.h"

#include "App.h"
#include "Render.h"

Particle::Particle()
{
	position.SetToZero();
	speed.SetToZero();
	
}

Particle::Particle(const Particle& p) : anim(p.anim), position(p.position), speed(p.speed),
										frameCount(p.frameCount), lifetime(p.lifetime), texture(p.texture)
{
	isAlive = false;
}

Particle::~Particle()
{

}

bool Particle::Update(float dt)
{
	bool ret = true;
	frameCount++;

	// The particle is set to 'alive' when the delay has been reached
	if (!isAlive && frameCount >= 0)
		isAlive = true;

	if (isAlive)
	{
		if (speed.x != 0.0f || speed.y != 0.0f) {
			auxPos.x += speed.x * dt;
			auxPos.y += speed.y * dt;
			position.x = auxPos.x;
			position.y = auxPos.y;
		}

		anim.Update();

		// If the particle has a specific lifetime, check when it has to be destroyed
		if (lifetime > 0)
		{
			if (frameCount >= lifetime)
				ret = false;
		}
		// Otherwise the particle is destroyed when the animation is finished
		else if (anim.HasFinished())
			ret = false;
	}

	return ret;
}

void Particle::SetToDelete()
{
	pendingToDelete = true;
}
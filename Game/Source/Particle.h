#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "Animation.h"
#include "Point.h"

struct SDL_Texture;

struct Particle
{
public:
	// Constructor
	Particle();

	// Copy constructor
	Particle(const Particle& p);

	// Destructor
	~Particle();

	// Called in ModuleParticles' Update
	// Handles the logic of the particle
	// Returns false when the particle reaches its lifetime
	bool Update(float dt);

	// Sets flag for deletion and for the collider aswell
	void SetToDelete();

public:
	// Defines the position in the screen
	iPoint position;
	fPoint auxPos = { 0.0f,0.0f };

	// Defines the speed at which the particle will move (pixels per second)
	fPoint speed;

	// A set of rectangle sprites
	Animation anim;

	// Defines wether the particle is alive or not
	// Particles will be set to not alive until "spawnTime" is reached
	bool isAlive = false;

	// Defines the amout of frames this particle has been active
	// Negative values mean the particle is waiting to be activated
	int frameCount = 0;

	// Defines the total amount of frames during which the particle will be active
	uint lifetime = 0;

	// A flag for the particle removal. Important! We do not delete objects instantly
	bool pendingToDelete = false;

	SDL_Texture* texture = nullptr;

	bool acceletarion = false;
};

#endif //__PARTICLE_H__
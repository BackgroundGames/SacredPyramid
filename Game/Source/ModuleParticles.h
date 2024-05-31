#ifndef __MODULE_PARTICLES_H__
#define __MODULE_PARTICLES_H__

#include "Module.h"

#include "Particle.h"
#include <random>

#define MAX_ACTIVE_PARTICLES 500

struct SDL_Texture;

class ModuleParticles : public Module
{
public:
	// Constructor
	// Initializes all the particles in the array to nullptr
	ModuleParticles();

	//Destructor
	~ModuleParticles();

	// Called when the module is activated
	// Loads the necessary textures for the particles
	bool Start() override;

	// Called at the beginning of the application loop
	// Removes all particles pending to delete
	bool PreUpdate() override;

	// Called at the middle of the application loop
	// Iterates all the particles and calls its Update()
	// Removes any "dead" particles
	bool Update(float dt) override;

	// Called at the end of the application loop
	// Iterates all the particles and draws them
	bool PostUpdate() override;

	// Called on application exit
	// Destroys all active particles left in the array
	bool CleanUp() override;

	// Creates a new particle and adds it to the array
	// Param particle	- A template particle from which the new particle will be created
	// Param x, y		- Position x,y in the screen (upper left axis)
	// Param delay		- Delay time from the moment the function is called until the particle is displayed in screen
	Particle* AddParticle(const Particle& particle, int x, int y, uint delay = 0);

	void PlaySandBreeze();
	void PlayLowRain();
	void StopWeather();

public:
	Particle laser;
	Particle weather;

	float minSpeedX = 0.005f;
	float maxSpeedX = 0.1f;
	float minSpeedY = -0.005f;
	float maxSpeedY = 0.005f;
	int minLifetime = 100;
	int maxLiftime = 500;

private:
	// Particles spritesheet loaded into an SDL Texture
	SDL_Texture* laserTexture = nullptr;

	// An array to store and handle all the particles
	Particle* particles[MAX_ACTIVE_PARTICLES] = { nullptr };

	//random floats
	std::mt19937 gen;
	std::uniform_real_distribution<> disf;
	std::uniform_int_distribution<> disi;

	Uint8 r, g, b;

	bool playWeather = false;
};

#endif // !__MODULEPARTICLES_H__
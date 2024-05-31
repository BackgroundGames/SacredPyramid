#include "ModuleParticles.h"

#include "App.h"

#include "Textures.h"
#include "Render.h"
#include "Map.h"

ModuleParticles::ModuleParticles() : Module()
{
	name.Create("moduleparticles");

	for(uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		particles[i] = nullptr;

	// Laser particle
	laser.anim.PushBack({ 0, 0, 50, 39 });
	laser.anim.PushBack({ 50, 0, 50, 39 });
	laser.anim.PushBack({ 100, 0, 50, 39 });
	laser.anim.PushBack({ 150, 0, 50, 39 });
	laser.anim.PushBack({ 200, 0, 50, 39 });
	laser.anim.PushBack({ 250, 0, 50, 39 });
	laser.anim.PushBack({ 300, 0, 50, 39 });
	laser.anim.PushBack({ 350, 0, 50, 39 });
	laser.anim.PushBack({ 400, 0, 50, 39 });
	laser.anim.PushBack({ 450, 0, 50, 39 });
	laser.anim.PushBack({ 500, 0, 50, 39 });
	laser.anim.PushBack({ 550, 0, 50, 39 });
	laser.anim.PushBack({ 600, 0, 50, 39 });
	laser.anim.PushBack({ 650, 0, 50, 39 });
	laser.anim.PushBack({ 700, 0, 50, 39 });
	laser.anim.PushBack({ 0, 39, 50, 39 });
	laser.anim.PushBack({ 50, 39, 50, 39 });
	laser.anim.PushBack({ 100, 39, 50, 39 });
	laser.anim.PushBack({ 150, 39, 50, 39 });
	laser.anim.PushBack({ 200, 39, 50, 39 });
	laser.anim.PushBack({ 250, 39, 50, 39 });
	laser.anim.PushBack({ 300, 39, 50, 39 });
	laser.anim.PushBack({ 350, 39, 50, 39 });
	laser.anim.PushBack({ 400, 39, 50, 39 });
	laser.anim.PushBack({ 450, 39, 50, 39 });
	laser.anim.PushBack({ 500, 39, 50, 39 });
	laser.anim.PushBack({ 550, 39, 50, 39 });
	laser.anim.PushBack({ 600, 39, 50, 39 });
	laser.anim.PushBack({ 650, 39, 50, 39 });
	laser.anim.PushBack({ 700, 39, 50, 39 });
	laser.anim.loop = true;
	laser.anim.speed = 0.5f;
	laser.lifetime = -1;
	//laser.speed.y = -20;

}

ModuleParticles::~ModuleParticles()
{

}

bool ModuleParticles::Start()
{
	laserTexture = app->tex->Load("Assets/Textures/laser.png");
	laser.texture = laserTexture;

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (particles[i] != nullptr)
		{
			delete particles[i];
			particles[i] = nullptr;
		}
	}

	// Crear un generador de números aleatorios
	gen.seed(std::random_device{}());

	return true;
}

bool ModuleParticles::PreUpdate()
{
	// Remove all particles scheduled for deletion
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (particles[i] != nullptr && particles[i]->pendingToDelete)
		{
 			delete particles[i];
			particles[i] = nullptr;
		}
	}

	return true;
}

bool ModuleParticles::CleanUp()
{
	// Delete all remaining active particles on application exit 
	for(uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if(particles[i] != nullptr)
		{
			delete particles[i];
			particles[i] = nullptr;
		}
		
	}
	app->tex->UnLoad(laserTexture);
	laserTexture = nullptr;

	return true;
}

bool ModuleParticles::Update(float dt)
{
	for(uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if(particle == nullptr)	continue;

		// Call particle Update. If it has reached its lifetime, destroy it
		if(particle->Update(dt) == false)
		{
			particles[i]->SetToDelete();
		}

		if (particle->isAlive)
		{
			if (particle->texture != nullptr) {
				app->render->DrawTexture(particle->texture, particle->position.x, particle->position.y - particle->anim.GetCurrentFrame().h / 2, &(particle->anim.GetCurrentFrame()));
			}
			else {
				SDL_Rect q;
				q.x = particle->position.x;
				q.y = particle->position.y;
				q.w = 3;
				q.h = 3;
				app->render->DrawRectangle(q, r, g, b);
			}
		}
	}

	if (playWeather) {
		for (size_t i = 0; i < 10; i++)
		{
			AddParticle(weather, 0, 0, 0);

		}
	}

	return true;
}

bool ModuleParticles::PostUpdate()
{
	return true;
}

Particle* ModuleParticles::AddParticle(const Particle& particle, int x, int y, uint delay)
{
	Particle* newParticle = nullptr;

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		//Finding an empty slot for a new particle
		if (particles[i] == nullptr)
		{
			newParticle = new Particle(particle);
			newParticle->frameCount = -(int)delay;			// We start the frameCount as the negative delay so when frameCount reaches 0 the particle will be activated
			newParticle->position = iPoint(app->map->MapToWorld(x, y));
			newParticle->position += iPoint((app->map->GetTileWidth() / 2) - (newParticle->anim.frames[0].w / 2), (app->map->GetTileHeight()) - (newParticle->anim.frames[0].h / 2));

			if (newParticle->texture == nullptr) {
				//random vel
				disf = std::uniform_real_distribution<>(maxSpeedX, minSpeedX);
				float random_float = disf(gen);
				newParticle->speed.x = random_float;
				disf = std::uniform_real_distribution<>(maxSpeedY, minSpeedY);
				random_float = disf(gen);
				newParticle->speed.y = random_float;

				//random pos
				disi = std::uniform_int_distribution<>((app->render->camera.x + 200) * -1, (app->render->camera.x - app->render->camera.w - 200) * -1);
				int random_int = disi(gen);
				newParticle->position.x = random_int;
				newParticle->auxPos.x = newParticle->position.x;
				disi = std::uniform_int_distribution<>((app->render->camera.y + 200) * -1, (app->render->camera.y - app->render->camera.h - 200)*-1);
				random_int = disi(gen);
				newParticle->position.y = random_int;
				newParticle->auxPos.y = newParticle->position.y;

				//life time
				disi = std::uniform_int_distribution<>(minLifetime, maxLiftime);
				random_int = disi(gen);
				newParticle->lifetime = random_int;
			}

			particles[i] = newParticle;
			break;
		}
	}

	return newParticle;
}

void ModuleParticles::PlaySandBreeze()
{
	minSpeedX = 0.005f;
	maxSpeedX = 0.1f;
	minSpeedY = -0.005f;
	maxSpeedY = 0.005f;
	minLifetime = 100;
	maxLiftime = 500;
	playWeather = true;
	r = 194;
	g = 178;
	b = 128;
}

void ModuleParticles::PlayLowRain()
{
	minSpeedX = 0.0f;
	maxSpeedX = 0.0f;
	minSpeedY = 0.5f;
	maxSpeedY = 0.5f;
	minLifetime = 50;
	maxLiftime = 50;
	r = 0;
	g = 150;
	b = 228;
	playWeather = true;
}

void ModuleParticles::StopWeather()
{
	playWeather = false;
}

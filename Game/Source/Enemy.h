#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Character.h"

class Enemy : public Character
{
public:
	
	Enemy();

	Enemy(uint hp, uint mp, uint atk, uint def, uint matk, uint mdef, uint ini);

	~Enemy();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

private:
	bool patrol = true;
	const DynArray<iPoint>* path;

public:
	EnemyType subtype;
};
#endif // !__ENEMY_H__


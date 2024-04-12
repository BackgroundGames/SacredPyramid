#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Character.h"
#include <vector>

using namespace std;

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

public:
	uint id;
	EnemyType subtype;
	vector<Entity*> enemies;
	bool assaulted = false;
	bool hasAttacked = false;
	vector<iPoint> combatPos;
	Character* pFocus = nullptr;
};
#endif // !__ENEMY_H__


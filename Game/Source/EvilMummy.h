#ifndef __EVILMUMMY_H__
#define __EVILMUMMY_H__

#include "Entity.h"
#include "Enemy.h"

class EvilMummy : public Enemy
{
public:
	EvilMummy();
	EvilMummy(Stats stats);

	~EvilMummy();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();
private:

};

#endif // __EVILMUMMY_H_
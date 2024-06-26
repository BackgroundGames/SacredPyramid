#ifndef __EVILMUMMY_H__
#define __EVILMUMMY_H__

#include "Enemy.h"

class EvilMummy : public Enemy
{
public:
	EvilMummy();
	EvilMummy(Stats stats, Inventory inventory);

	~EvilMummy();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();
private:

};

#endif // __EVILMUMMY_H_
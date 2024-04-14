#ifndef __BANDIT_H__
#define __BANDIT_H__

#include "Enemy.h"

class Bandit : public Enemy
{
public:
	Bandit();
	Bandit(Stats stats);

	~Bandit();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();
private:

};

#endif // __BANDIT_H_
#ifndef __DRUNKARD_H__
#define __DRUNKARD_H__

#include "Enemy.h"

class Drunkard : public Enemy
{
public:
	Drunkard();

	~Drunkard();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();
private:

};

#endif // __DRUNKARD_H_
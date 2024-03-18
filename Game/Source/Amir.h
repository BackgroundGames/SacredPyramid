#ifndef __AMIR_H__
#define __AMIR_H__


#include "Player.h"


class Amir : public Player
{
public:

	Amir();

	Amir(Stats stats);

	~Amir();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();
};

#endif // !__Amir_H__


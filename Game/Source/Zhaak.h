#ifndef __ZHAAK_H__
#define __ZHAAK_H__


#include "Player.h"


class Zhaak : public Player 
{
public:

	Zhaak();

	Zhaak(Stats stats, Equipment inventory);

	~Zhaak();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:
	
};

#endif // !__Zhaak_H__


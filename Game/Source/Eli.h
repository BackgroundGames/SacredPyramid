#ifndef __ELI_H__
#define __ELI_H__


#include "Player.h"


class Eli : public Player
{
public:

	Eli();

	Eli(Stats stats);

	~Eli();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();
};

#endif // !__ELI_H__


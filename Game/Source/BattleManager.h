#pragma once

#include "Player.h"
#include "Enemy.h"
#include "Character.h"
#include "List.h"

class BattleManager
{
	void startBattle(List<Character>& allCharacters);

	bool isBattleOver(List<Character>& allies, List<Character>& enemies);

	bool isPartyDefeated(List<Character>& party);

	void characterTurn(Character& character, List<Character>& allCharacters);
};
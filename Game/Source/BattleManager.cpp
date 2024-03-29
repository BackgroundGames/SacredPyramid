#include "BattleManager.h"
#include "Character.h"

void BattleManager::startBattle(List<Character>& allCharacters)
{
	List<Character> allies;
	List<Character> enemies;

	ListItem<Character>* character = allCharacters.start;
	while (character != NULL) 
	{
		if (character->data.type == EntityType::ENEMY) 
		{
			enemies.Add(character->data);
		}
		else 
		{
			allies.Add(character->data);
		}
		character = character->next;
	}

	allCharacters.BubbleSortTheInitiative();

	while (!isBattleOver(allies, enemies)) 
	{
		ListItem<Character>* currentCharacter = allCharacters.start;
		while (currentCharacter != nullptr) 
		{
			if (currentCharacter->data.GetHealth() > 0) 
			{
				characterTurn(currentCharacter->data, allCharacters);
			}
			currentCharacter = currentCharacter->next;
		}
	}
}

bool BattleManager::isBattleOver(List<Character>& allies, List<Character>& enemies)
{
	return isPartyDefeated(allies) || isPartyDefeated(enemies);
}

bool BattleManager::isPartyDefeated(List<Character>& party)
{
	ListItem<Character>* currentCharacter = party.start;

	while (currentCharacter != NULL) 
	{
		if (currentCharacter->data.GetHealth() > 0) 
		{
			return false;
		}
		currentCharacter = currentCharacter->next;
	}
	return true;
}

void BattleManager::characterTurn(Character& character, List<Character>& allCharacters)
{

}

//Damage formula
//(Character attack + Character weapon damage) - (Target defense + Target armor defense)

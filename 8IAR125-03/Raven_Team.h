#pragma once

#include <list>
#include "Raven_Bot.h"
#include "Raven_Game.h"

class Raven_Team
{
public:
	enum { Blue, Red, Green, Yellow, Black }; // Available team colors
	static const int MAX_TEAMS = 5;

private:
	std::list<Raven_Bot*> m_Bots;	// The bots in the team
	std::list<Vector2D> m_DroppedWeapons;	// The dropped weapons
	int m_Id;					// The team id
	Raven_Game* m_pWorld;		// The game world

public:
	Raven_Team(int id, Raven_Game* world);

	void AddMember(Raven_Bot* bot);
	void RemoveMember(Raven_Bot* bot);

	void AddDroppedWeapon(Vector2D pos);
	void RemoveDroppedWeapon(Vector2D pos);

	int GetNbMembers() const { return m_Bots.size(); };
	int GetId() const { return m_Id; }
	Raven_Game* GetWorld() const { return m_pWorld; }

	void Update();

	static void PenColor(int id);
	static void BrushColor(int id);
};


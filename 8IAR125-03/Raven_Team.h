#pragma once

#include <list>
#include "Raven_Bot.h"

class Raven_Team
{
public:
	enum { Blue, Red, Green, Yellow, Grey }; // Available team colors
	static const int MAX_TEAMS = 5;

private:
	std::list<Raven_Bot*> m_Bots;	// The bots in the team
	int m_Color;					// The team color

public:
	Raven_Team(int color);

	void AddMember(Raven_Bot* bot);
	void RemoveMember(Raven_Bot* bot);

	int GetNbMembers() const { return m_Bots.size(); };
	int GetColor() const { return m_Color; }

	void Update();
};


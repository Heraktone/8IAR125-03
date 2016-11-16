#pragma once

#include <list>
#include "Raven_Bot.h"

class Raven_Team
{
public:
	enum { Blue, Red, Green, Yellow, Black }; // Available team colors
	static const int MAX_TEAMS = 5;

private:
	std::list<Raven_Bot*> m_Bots;	// The bots in the team
	int m_Id;					// The team id

public:
	Raven_Team(int id);

	void AddMember(Raven_Bot* bot);
	void RemoveMember(Raven_Bot* bot);

	int GetNbMembers() const { return m_Bots.size(); };
	int GetId() const { return m_Id; }

	void Update();

	static void PenColor(int id);
	static void BrushColor(int id);
};


#pragma once

#include <list>
#include "Raven_Bot.h"
#include "Raven_Game.h"

class Raven_Team
{
public:
	enum { Blue, Red, Green, Yellow, Black }; // Available team colors
	enum Behaviour {Normal, Reckless, Careful};
	static const int MAX_TEAMS = 5;
	static const int NB_BEHAVIOURS = 3;

private:
	std::list<Raven_Bot*> m_Bots;	// The bots in the team
	std::list<Vector2D> m_DroppedWeapons;	// The dropped weapons
	int m_Id;					// The team id
	Raven_Game* m_pWorld;		// The game world

	double HealthBias;
	double ShotgunBias;
	double RocketLauncherBias;
	double RailgunBias;
	double KnifeBias;
	double ExploreBias;
	double AttackBias;

public:
	Raven_Team(int id, Raven_Game* world, int behaviour);

	double GetHealthBias() { return HealthBias; }
	double GetShotgunBias() { return ShotgunBias; }
	double GetRocketLauncherBias() { return RocketLauncherBias; }
	double GetRailgunBias() { return RailgunBias; }
	double GetKnifeBias() { return KnifeBias; }
	double GetExploreBias() { return ExploreBias; }
	double GetAttackBias() { return AttackBias; }

	void AddMember(Raven_Bot* bot);
	void RemoveMember(Raven_Bot* bot);

	void AddDroppedWeapon(Vector2D pos);
	void RemoveDroppedWeapon(Vector2D pos);

	int GetNbMembers() const { return m_Bots.size(); };
	int GetId() const { return m_Id; }
	Raven_Game* GetWorld() const { return m_pWorld; }

	std::list<int> GetMembersID();

	void Update();

	static void PenColor(int id);
	static void BrushColor(int id);
};


#include "Trigger_DroppedWeapon.h"
#include "../lua/Raven_Scriptor.h"
#include "../Raven_ObjectEnumerations.h"
#include "../Raven_Team.h"
#include "../Raven_Bot.h"
#include "../Raven_WeaponSystem.h"

// Constructor
Trigger_DroppedWeapon::Trigger_DroppedWeapon(Vector2D pos, unsigned int weapon, int ammo, int team, Raven_Game* world) :
	Trigger<Raven_Bot>(BaseGameEntity::GetNextValidID()),
	m_WeaponType(weapon),
	m_Ammo(ammo),
	m_Team(team),
	m_pWorld(world)
{
	SetPos(pos);
	AddCircularTriggerRegion(pos, 5);

	//create the vertex buffer for the rocket shape
	const int NumRocketVerts = 8;
	const Vector2D rip[NumRocketVerts] = { Vector2D(0, 3),
		Vector2D(1, 2),
		Vector2D(1, 0),
		Vector2D(2, -2),
		Vector2D(-2, -2),
		Vector2D(-1, 0),
		Vector2D(-1, 2),
		Vector2D(0, 3) };

	for (int i = 0; i<NumRocketVerts; ++i)
	{
		m_vecRLVB.push_back(rip[i]);
	}
}

// Update
void Trigger_DroppedWeapon::Update() {
	if (!m_pWorld->TeamsActivated())
	{
		SetToBeRemovedFromGame();
	}
}

// If the item is picked, then erase it
void Trigger_DroppedWeapon::Try(Raven_Bot* bot) {
	if (m_pWorld->TeamsActivated()) {
		if (this->isActive() && bot->GetTeam()->GetId() == m_Team && this->isTouchingTrigger(bot->Pos(), bot->BRadius()))
		{
			bot->GetWeaponSys()->AddWeapon(EntityType());
			bot->GetTeam()->RemoveDroppedWeapon(m_vPosition);
			SetToBeRemovedFromGame();
		}
	}
}

// Render the weapon
void Trigger_DroppedWeapon::Render()
{
	if (isActive())
	{
		Raven_Team::PenColor(m_Team);
		gdi->BlackBrush();
		gdi->Circle(m_vPosition, 5);
	}
}
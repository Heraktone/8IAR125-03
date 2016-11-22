#include "Weapon_Knife.h"
#include "../Raven_Bot.h"
#include "misc/Cgdi.h"
#include "../Raven_Game.h"
#include "../Raven_Map.h"
#include "../lua/Raven_Scriptor.h"
#include "fuzzy/FuzzyOperators.h"


//--------------------------- ctor --------------------------------------------
//-----------------------------------------------------------------------------
Knife::Knife(Raven_Bot*   owner) :

	Raven_Weapon(type_knife,
		script->GetInt("Knife_DefaultRounds"),
		script->GetInt("Knife_MaxRoundsCarried"),
		script->GetDouble("Knife_FiringFreq"),
		script->GetDouble("Knife_IdealRange"),
		script->GetDouble("Knifes_MaxSpeed"), 
		owner)
{

	//setup the vertex buffer
	const int NumWeaponVerts = 4;
	const Vector2D weapon[NumWeaponVerts] = { Vector2D(0, -1),
		Vector2D(10, -1),
		Vector2D(10, 1),
		Vector2D(0, 1)
	};


	for (int vtx = 0; vtx<NumWeaponVerts; ++vtx)
	{
		m_vecWeaponVB.push_back(weapon[vtx]);
	}

	//setup the fuzzy module
	InitializeFuzzyModule();
}


//------------------------------ ShootAt --------------------------------------

inline void Knife::ShootAt(Vector2D pos)
{
	if (NumRoundsRemaining() > 0 && isReadyForNextShot())
	{
		//fire a round
		m_pOwner->GetWorld()->AddKnifes(m_pOwner, pos);

		UpdateTimeWeaponIsNextAvailable();

		m_iNumRoundsLeft--;
	}
}

//---------------------------- Desirability -----------------------------------
//
//-----------------------------------------------------------------------------
double Knife::GetDesirability(double DistToTarget)
{
	if (m_iNumRoundsLeft == 0)
	{
		m_dLastDesirabilityScore = 0;
	}
	else
	{
		//fuzzify distance and amount of ammo
		m_FuzzyModule.Fuzzify("DistToTarget", DistToTarget);

		m_dLastDesirabilityScore = m_FuzzyModule.DeFuzzify("Desirability", FuzzyModule::max_av);

		return m_dLastDesirabilityScore;
	}

	return m_dLastDesirabilityScore;
}

//----------------------- InitializeFuzzyModule -------------------------------
//
//  set up some fuzzy variables and rules
//-----------------------------------------------------------------------------
void Knife::InitializeFuzzyModule()
{
	FuzzyVariable& DistToTarget = m_FuzzyModule.CreateFLV("DistToTarget");

	FzSet& Target_Close = DistToTarget.AddLeftShoulderSet("Target_Close", 0, 25, 150);
	FzSet& Target_Medium = DistToTarget.AddTriangularSet("Target_Medium", 25, 150, 300);
	FzSet& Target_Far = DistToTarget.AddRightShoulderSet("Target_Far", 150, 300, 1000);

	FuzzyVariable& Desirability = m_FuzzyModule.CreateFLV("Desirability");
	FzSet& VeryDesirable = Desirability.AddRightShoulderSet("VeryDesirable", 50, 75, 100);
	FzSet& Desirable = Desirability.AddTriangularSet("Desirable", 25, 50, 75);
	FzSet& Undesirable = Desirability.AddLeftShoulderSet("Undesirable", 0, 25, 50);

	m_FuzzyModule.AddRule(Target_Close, VeryDesirable);
	m_FuzzyModule.AddRule(Target_Medium, FzVery(Undesirable));
	m_FuzzyModule.AddRule(Target_Far, FzVery(Undesirable));
}

//-------------------------------- Render -------------------------------------
//-----------------------------------------------------------------------------
void Knife::Render()
{
	m_vecWeaponVBTrans = WorldTransform(m_vecWeaponVB,
		m_pOwner->Pos(),
		m_pOwner->Facing(),
		m_pOwner->Facing().Perp(),
		m_pOwner->Scale());

	gdi->PinkPen();

	gdi->ClosedShape(m_vecWeaponVBTrans);
}
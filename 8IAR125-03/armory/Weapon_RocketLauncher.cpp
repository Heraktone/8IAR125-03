#include "Weapon_RocketLauncher.h"
#include "../Raven_Bot.h"
#include "misc/Cgdi.h"
#include "../Raven_Game.h"
#include "../Raven_Map.h"
#include "../lua/Raven_Scriptor.h"
#include "fuzzy/FuzzyOperators.h"


//--------------------------- ctor --------------------------------------------
//-----------------------------------------------------------------------------
RocketLauncher::RocketLauncher(Raven_Bot*   owner):

                      Raven_Weapon(type_rocket_launcher,
                                   script->GetInt("RocketLauncher_DefaultRounds"),
                                   script->GetInt("RocketLauncher_MaxRoundsCarried"),
                                   script->GetDouble("RocketLauncher_FiringFreq"),
                                   script->GetDouble("RocketLauncher_IdealRange"),
                                   script->GetDouble("Rocket_MaxSpeed"),
                                   owner)
{
    //setup the vertex buffer
  const int NumWeaponVerts = 8;
  const Vector2D weapon[NumWeaponVerts] = {Vector2D(0, -3),
                                           Vector2D(6, -3),
                                           Vector2D(6, -1),
                                           Vector2D(15, -1),
                                           Vector2D(15, 1),
                                           Vector2D(6, 1),
                                           Vector2D(6, 3),
                                           Vector2D(0, 3)
                                           };
  for (int vtx=0; vtx<NumWeaponVerts; ++vtx)
  {
    m_vecWeaponVB.push_back(weapon[vtx]);
  }

  //setup the fuzzy module
  InitializeFuzzyModule();

}


//------------------------------ ShootAt --------------------------------------
//-----------------------------------------------------------------------------
inline void RocketLauncher::ShootAt(Vector2D pos)
{ 
  if (NumRoundsRemaining() > 0 && isReadyForNextShot())
  {
    //fire off a rocket!
    m_pOwner->GetWorld()->AddRocket(m_pOwner, pos);

    m_iNumRoundsLeft--;

    UpdateTimeWeaponIsNextAvailable();

    //add a trigger to the game so that the other bots can hear this shot
    //(provided they are within range)
    m_pOwner->GetWorld()->GetMap()->AddSoundTrigger(m_pOwner, script->GetDouble("RocketLauncher_SoundRange"));
  }
}

//---------------------------- Desirability -----------------------------------
//
//-----------------------------------------------------------------------------
double RocketLauncher::GetDesirability(double DistToTarget)
{
  if (m_iNumRoundsLeft == 0)
  {
    m_dLastDesirabilityScore = 0;
  }
  else
  {
    //fuzzify distance and amount of ammo
    m_FuzzyModule.Fuzzify("DistToTarget", DistToTarget);
    m_FuzzyModule.Fuzzify("AmmoStatus", (double)m_iNumRoundsLeft);

    m_dLastDesirabilityScore = m_FuzzyModule.DeFuzzify("Desirability", FuzzyModule::max_av);
  }

  return m_dLastDesirabilityScore;
}

//-------------------------  InitializeFuzzyModule ----------------------------
//
//  set up some fuzzy variables and rules
//-----------------------------------------------------------------------------
void RocketLauncher::InitializeFuzzyModule()
{
  FuzzyVariable& DistToTarget = m_FuzzyModule.CreateFLV("DistToTarget");

  FzSet& Target_Close = DistToTarget.AddLeftShoulderSet("Target_Close",0,25,100);
  FzSet& Target_Mid_Close = DistToTarget.AddTriangularSet("Target_Mid_Close", 25, 100, 150);
  FzSet& Target_Medium = DistToTarget.AddTriangularSet("Target_Medium",100,150,200);
  FzSet& Target_Mid_Far = DistToTarget.AddTriangularSet("Target_Mid_Far", 150, 200, 275);
  FzSet& Target_Far = DistToTarget.AddRightShoulderSet("Target_Far",200,275,1000);

  FuzzyVariable& Desirability = m_FuzzyModule.CreateFLV("Desirability");
  FzSet& Undesirable = Desirability.AddLeftShoulderSet("Undesirable", 0, 20, 35);
  FzSet& Fairly_Desirable = DistToTarget.AddTriangularSet("Fairly_Desirable", 20, 35, 50);
  FzSet& Desirable = Desirability.AddTriangularSet("Desirable", 35, 50, 65);
  FzSet& Very_Desirable = DistToTarget.AddTriangularSet("Very_Desirable", 50, 65, 80);
  FzSet& Very_Very_Desirable = Desirability.AddRightShoulderSet("Very_Very_Desirable", 65, 80, 100);

  FuzzyVariable& AmmoStatus = m_FuzzyModule.CreateFLV("AmmoStatus");
  FzSet& Ammo_Low = AmmoStatus.AddTriangularSet("Ammo_Low", 0, 0, 10);
  FzSet& Ammo_Not_Much = AmmoStatus.AddTriangularSet("Ammo_Not_Much", 0, 10, 20);
  FzSet& Ammo_Okay = AmmoStatus.AddTriangularSet("Ammo_Okay", 10, 20, 30);
  FzSet& Ammo_Enough = AmmoStatus.AddTriangularSet("Ammo_Enough", 20, 30, 35);
  FzSet& Ammo_Loads = AmmoStatus.AddRightShoulderSet("Ammo_Loads", 30, 35, 100);

  m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Low), Undesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Not_Much), Undesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Okay), Undesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Enough), Undesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Loads), Undesirable);

  m_FuzzyModule.AddRule(FzAND(Target_Mid_Close, Ammo_Low), Fairly_Desirable);
  m_FuzzyModule.AddRule(FzAND(Target_Mid_Close, Ammo_Not_Much), Fairly_Desirable);
  m_FuzzyModule.AddRule(FzAND(Target_Mid_Close, Ammo_Okay), Desirable);
  m_FuzzyModule.AddRule(FzAND(Target_Mid_Close, Ammo_Enough), Desirable);
  m_FuzzyModule.AddRule(FzAND(Target_Mid_Close, Ammo_Loads), Desirable);

  m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Low), Desirable);
  m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Not_Much), Very_Desirable);
  m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Okay), Very_Very_Desirable);
  m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Enough), Very_Very_Desirable);
  m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Loads), Very_Very_Desirable);

  m_FuzzyModule.AddRule(FzAND(Target_Mid_Far, Ammo_Low), Fairly_Desirable);
  m_FuzzyModule.AddRule(FzAND(Target_Mid_Far, Ammo_Not_Much), Fairly_Desirable);
  m_FuzzyModule.AddRule(FzAND(Target_Mid_Far, Ammo_Okay), Desirable);
  m_FuzzyModule.AddRule(FzAND(Target_Mid_Far, Ammo_Enough), Very_Desirable);
  m_FuzzyModule.AddRule(FzAND(Target_Mid_Far, Ammo_Loads), Very_Desirable);

  m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Low), Undesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Not_Much), Undesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Okay), Undesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Enough), Fairly_Desirable);
  m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Loads), Desirable);
}


//-------------------------------- Render -------------------------------------
//-----------------------------------------------------------------------------
void RocketLauncher::Render()
{
    m_vecWeaponVBTrans = WorldTransform(m_vecWeaponVB,
                                   m_pOwner->Pos(),
                                   m_pOwner->Facing(),
                                   m_pOwner->Facing().Perp(),
                                   m_pOwner->Scale());

  gdi->RedPen();

  gdi->ClosedShape(m_vecWeaponVBTrans);
}
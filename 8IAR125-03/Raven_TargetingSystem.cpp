#include "Raven_TargetingSystem.h"
#include "Raven_Bot.h"
#include "Raven_SensoryMemory.h"
#include "Raven_Game.h"
#include "../Common/Game/EntityManager.h"



//-------------------------------- ctor ---------------------------------------
//-----------------------------------------------------------------------------
Raven_TargetingSystem::Raven_TargetingSystem(Raven_Bot* owner):m_pOwner(owner),
                                                               m_pCurrentTarget(0)
{}



//----------------------------- Update ----------------------------------------

//-----------------------------------------------------------------------------
void Raven_TargetingSystem::Update()
{
 // double ClosestDistSoFar = MaxDouble;
	double lowCost = MaxDouble;
  m_pCurrentTarget       = 0;
  double coeff = 100;

  

  //grab a list of all the opponents the owner can sense
  std::list<Raven_Bot*> SensedBots;
  SensedBots = m_pOwner->GetSensoryMem()->GetListOfRecentlySensedOpponents();

  
  std::list<Raven_Bot*>::const_iterator curBot = SensedBots.begin();
  for (curBot; curBot != SensedBots.end(); ++curBot)
  {
    //make sure the bot is alive and that it is neither the owner nor a team member
    if ((*curBot)->isAlive() && (*curBot != m_pOwner))
    {
		if (!(*curBot)->GetWorld()->TeamsActivated() || (*curBot)->GetTeam() != m_pOwner->GetTeam())
		{

			double dist = Vec2DDistanceSq((*curBot)->Pos(), m_pOwner->Pos());
			// compute a bonus if the enemy is aldready attacked 
			double bonus = 0;
			std::list<int> alliesIDs = this->m_pOwner->GetTeam()->GetMembersID();
			std::list<int>::iterator nextAlly = alliesIDs.begin();
			for (nextAlly; nextAlly != alliesIDs.end(); ++nextAlly) {
				if (*nextAlly != this->m_pOwner->ID()) {
					Raven_Bot* curAlly = (Raven_Bot*)EntityManager::Instance()->GetEntityFromID(*nextAlly);
					if (curAlly->ID() == (*curBot)->ID()) {
						bonus -= 1 / dist * 1/(*curBot)->Health() * coeff;
					}
					
				}
			}

			

			if ((dist + bonus) < lowCost)
			{
				lowCost = dist + bonus;
				m_pCurrentTarget = *curBot;
			}
		}
    }
  }
}




bool Raven_TargetingSystem::isTargetWithinFOV()const
{
  return m_pOwner->GetSensoryMem()->isOpponentWithinFOV(m_pCurrentTarget);
}

bool Raven_TargetingSystem::isTargetShootable()const
{
  return m_pOwner->GetSensoryMem()->isOpponentShootable(m_pCurrentTarget);
}

Vector2D Raven_TargetingSystem::GetLastRecordedPosition()const
{
  return m_pOwner->GetSensoryMem()->GetLastRecordedPositionOfOpponent(m_pCurrentTarget);
}

double Raven_TargetingSystem::GetTimeTargetHasBeenVisible()const
{
  return m_pOwner->GetSensoryMem()->GetTimeOpponentHasBeenVisible(m_pCurrentTarget);
}

double Raven_TargetingSystem::GetTimeTargetHasBeenOutOfView()const
{
  return m_pOwner->GetSensoryMem()->GetTimeOpponentHasBeenOutOfView(m_pCurrentTarget);
}

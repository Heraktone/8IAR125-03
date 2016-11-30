#include "HelpGoal_Evaluator.h"

#include "AttackTargetGoal_Evaluator.h"
#include "Goal_Think.h"
#include "Raven_Goal_Types.h"
#include "../Raven_WeaponSystem.h"
#include "../Raven_ObjectEnumerations.h"
#include "misc/cgdi.h"
#include "misc/Stream_Utility_Functions.h"
#include "Raven_Feature.h"
#include "../Raven_Game.h"
#include "../Common/Game/EntityManager.h"


#include "debug/DebugConsole.h"

//------------------ CalculateDesirability ------------------------------------
//
//  returns a value between 0 and 1 that indicates the Rating of a bot (the
//  higher the score, the stronger the bot).
//-----------------------------------------------------------------------------
double HelpGoal_Evaluator::CalculateDesirability(Raven_Bot* pBot)
{
	double Desirability = 0.0;
	Goal_Think* brain = pBot->GetBrain();

	if (brain->DoesSomeoneNeedHelp()){
		Raven_Bot* toHelp = (Raven_Bot*)EntityManager::Instance()->GetEntityFromID(brain->toHelp());
		bool pasSoi = (brain->toHelp() != pBot->ID());
		if (pasSoi && (toHelp->GetTargetSys()->isTargetPresent()) && !(toHelp->isDead()))
		{
			//int id = brain->toHelp();
			//Raven_Bot* toHelp = (Raven_Bot*)EntityManager::Instance()->GetEntityFromID(id);
			//if ((pBot->GetWorld()->isSecondVisibleToFirst(pBot,toHelp)) && (pBot->Health() >= 50)) {
			if (pBot->Health() >= 50) {
				Desirability = 1.0;
			}
		}
	}

	return Desirability;
}

//----------------------------- SetGoal ---------------------------------------
//-----------------------------------------------------------------------------
void HelpGoal_Evaluator::SetGoal(Raven_Bot* pBot)
{

	Goal_Think* brain = pBot->GetBrain();
	Raven_Bot* toHelp = (Raven_Bot*)EntityManager::Instance()->GetEntityFromID(brain->toHelp());
	if (toHelp->GetTargetSys()->isTargetPresent() && !toHelp->isDead()) {
		Vector2D targetPos = (toHelp->Pos() + toHelp->GetTargetBot()->Pos()) / 2;
		pBot->GetBrain()->AddGoal_MoveToPosition(targetPos);
	}

}

//-------------------------- RenderInfo ---------------------------------------
//-----------------------------------------------------------------------------
void HelpGoal_Evaluator::RenderInfo(Vector2D Position, Raven_Bot* pBot)
{
	gdi->TextAtPos(Position, "AT: " + ttos(CalculateDesirability(pBot), 2));
	return;

	std::string s = ttos(Raven_Feature::Health(pBot)) + ", " + ttos(Raven_Feature::TotalWeaponStrength(pBot));
	gdi->TextAtPos(Position + Vector2D(0, 12), s);
}
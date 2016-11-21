#include "Goal_AlongWalls.h"
#include "Goal_SeekToPosition.h"
#include "../Raven_Bot.h"
#include "../Raven_SteeringBehaviors.h"
#include "../Raven_Game.h"

#include "Messaging/Telegram.h"
#include "../Raven_Messages.h"

#include "debug/DebugConsole.h"
#include "misc/cgdi.H"


//------------------------------- Activate ------------------------------------
//-----------------------------------------------------------------------------
void Goal_AlongWalls::Activate()
{
   m_iStatus = active;
   
}



//-------------------------- Process ------------------------------------------
//-----------------------------------------------------------------------------
int Goal_AlongWalls::Process()
{
  //if status is inactive, call Activate()
  ActivateIfInactive(); 



  return m_iStatus;
}

//---------------------------- Terminate --------------------------------------
//-----------------------------------------------------------------------------
void Goal_AlongWalls::Terminate()
{

}

//---------------------------- Render -----------------------------------------

void Goal_AlongWalls::Render()
{
//#define SHOW_TARGET
#ifdef SHOW_TARGET
  gdi->OrangePen();
  gdi->HollowBrush();

  gdi->Line(m_pOwner->Pos(), m_vStrafeTarget);
  gdi->Circle(m_vStrafeTarget, 3);
#endif
  
}






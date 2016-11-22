#include "Goal_TraverseEdge.h"
#include "..\Raven_Bot.h"
#include "Raven_Goal_Types.h"
#include "..\Raven_SteeringBehaviors.h"
#include "time/CrudeTimer.h"
#include "..\constants.h"
#include "../navigation/Raven_PathPlanner.h"
#include "misc/cgdi.h"
#include "../lua/Raven_Scriptor.h"


#include "debug/DebugConsole.h"




//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
Goal_TraverseEdge::Goal_TraverseEdge(Raven_Bot* pBot,
                                     PathEdge   edge,
                                     bool       LastEdge, bool clock, bool dodge):

                                Goal<Raven_Bot>(pBot, goal_traverse_edge),
                                m_Edge(edge),
                                m_dTimeExpected(0.0),
                                m_bLastEdgeInPath(LastEdge),
								m_clock(clock),
								m_halfway(false),
								m_dodge(dodge)
                                
{}

                            
                                             
//---------------------------- Activate -------------------------------------
//-----------------------------------------------------------------------------  
void Goal_TraverseEdge::Activate()
{
  m_iStatus = active;
  
  //the edge behavior flag may specify a type of movement that necessitates a 
  //change in the bot's max possible speed as it follows this edge
  switch(m_Edge.Behavior())
  {
    case NavGraphEdge::swim:
    {
      m_pOwner->SetMaxSpeed(script->GetDouble("Bot_MaxSwimmingSpeed"));
    }
   
    break;
   
    case NavGraphEdge::crawl:
    {
       m_pOwner->SetMaxSpeed(script->GetDouble("Bot_MaxCrawlingSpeed"));
    }
   
    break;
  }
  

  //record the time the bot starts this goal
  m_dStartTime = Clock->GetCurrentTime();   
  
 
  destination = m_Edge.Destination();

  // if dodging is activated, dodge to one side according to current clock, halfway through the edge
  if (m_pOwner->GetTargetSys()->GetTarget() != nullptr && !m_halfway && m_dodge) {
	  Vector2D direct = destination - m_pOwner->Pos(); // director vector
	  Vector2D inter_dest = direct / 2.0f + m_pOwner->Pos(); // halfway position

	  direct.Normalize();
	  direct = direct * 15; 

	  // according to current clock, dodge to one side with normal vector
	  if (m_clock) {
		  destination = inter_dest + Vector2D(-direct.y, direct.x);
	  }
	  else {
		  destination = inter_dest + Vector2D(direct.y, -direct.x);  
	  } 
	  
	  // if path is blocked, return to old edge destination
	  if (!m_pOwner->canWalkTo(destination)) {
		  destination = m_Edge.Destination();
	  }
  }
 
  //calculate the expected time required to reach the this waypoint. This value
  //is used to determine if the bot becomes stuck 
  m_dTimeExpected = m_pOwner->CalculateTimeToReachPosition(destination);

  //factor in a margin of error for any reactive behavior
  static const double MarginOfError = 2.0;

  m_dTimeExpected += MarginOfError;

  //set the steering target
  m_pOwner->GetSteering()->SetTarget(destination);
  
  //Set the appropriate steering behavior. If this is the last edge in the path
  //the bot should arrive at the position it points to, else it should seek
  if (m_bLastEdgeInPath)
  {
     m_pOwner->GetSteering()->ArriveOn();
  }

  else
  {
    m_pOwner->GetSteering()->SeekOn();
  }
}

//------------------------------ Process --------------------------------------
//-----------------------------------------------------------------------------
int Goal_TraverseEdge::Process()
{
  //if status is inactive, call Activate()
  ActivateIfInactive();
  
  //if the bot has become stuck return failure
  if (isStuck())
  {
    m_iStatus = failed;
  }
  
  //if the bot has reached the end of the edge return completed
  else
  { 

    if (m_pOwner->isAtPosition(m_Edge.Destination()))
    {
      m_iStatus = completed;
    }
	// if halfway reached, go to edge destination
	else  if (m_pOwner->isAtPosition(destination))
	{
		m_halfway = true;
		Activate();
	}
  }

  return m_iStatus;
}

//--------------------------- isBotStuck --------------------------------------
//
//  returns true if the bot has taken longer than expected to reach the 
//  currently active waypoint
//-----------------------------------------------------------------------------
bool Goal_TraverseEdge::isStuck()const
{  
  double TimeTaken = Clock->GetCurrentTime() - m_dStartTime;

  if (TimeTaken > m_dTimeExpected)
  {
    debug_con << "BOT " << m_pOwner->ID() << " IS STUCK!!" << "";

    return true;
  }

  return false;
}


//---------------------------- Terminate --------------------------------------
//-----------------------------------------------------------------------------
void Goal_TraverseEdge::Terminate()
{
  //turn off steering behaviors.
  m_pOwner->GetSteering()->SeekOff();
  m_pOwner->GetSteering()->ArriveOff();

  //return max speed back to normal
  m_pOwner->SetMaxSpeed(script->GetDouble("Bot_MaxSpeed"));
}

//----------------------------- Render ----------------------------------------
//-----------------------------------------------------------------------------
void Goal_TraverseEdge::Render()
{
  if (m_iStatus == active)
  {
    gdi->BluePen();
    //gdi->Line(m_pOwner->Pos(), m_Edge.Destination());
	gdi->Line(m_pOwner->Pos(), destination);
    gdi->GreenBrush();
    gdi->BlackPen();
    gdi->Circle(m_Edge.Destination(), 3);
	gdi->Circle(destination, 3);
  }
}


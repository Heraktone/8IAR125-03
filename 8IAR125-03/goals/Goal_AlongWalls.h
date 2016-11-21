#ifndef GOAL_ALONG_H
#define GOAL_ALONG_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Goal_DodgeSideToSide.h
//
//  Author: Mat Buckland (ai-junkie.com)
//
//  Desc:   this goal makes the bot dodge from side to side
//
//-----------------------------------------------------------------------------
#include "Goals/Goal.h"
#include "Raven_Goal_Types.h"
#include "../Raven_Bot.h"





class Goal_AlongWalls : public Goal<Raven_Bot>
{
private:

	int m_iItemToGet;

  


public:

	Goal_AlongWalls(Raven_Bot* pBot, int item):Goal<Raven_Bot>(pBot, goal_alongwalls),
		m_iItemToGet(item)
   {}


  void Activate();

  int  Process();

  void Render();

  void Terminate();
 

};






#endif

#pragma once
#include "Goal_Evaluator.h"
#include "../Raven_Bot.h"

class HelpGoal_Evaluator :
	public Goal_Evaluator
{
public:
	HelpGoal_Evaluator(double bias) :Goal_Evaluator(bias) {}

	double CalculateDesirability(Raven_Bot* pBot);

	void  SetGoal(Raven_Bot* pEnt);

	void RenderInfo(Vector2D Position, Raven_Bot* pBot);
};


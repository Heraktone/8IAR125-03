#ifndef GRENADE_H
#define GRENADE_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Rocket.h
//
//  Author: Mat Buckland (ai-junkie.com)
//
//  Desc:   class to implement a rocket
//
//-----------------------------------------------------------------------------

#include "Raven_Projectile.h"
#include <thread>


class Raven_Bot;

class Grenade : public Raven_Projectile
{
private:

	//the radius of damage, once the rocket has impacted
	double    m_dBlastRadius;

	//this is used to render the splash when the rocket impacts
	double    m_dCurrentBlastRadius;

	bool detonate;

	bool detonationStart;

	//If the rocket has impacted we test all bots to see if they are within the 
	//blast radius and reduce their health accordingly
	void InflictDamageOnBotsWithinBlastRadius();

	//tests the trajectory of the shell for an impact
	void TestForImpact();

	std::thread* m_waitThread;

public:

	Grenade(Raven_Bot* shooter, Vector2D target);

	virtual ~Grenade();

	void Render();

	void Update();

	void Detonate();

};


#endif
#ifndef KINFE_H
#define KINFE_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Weapon_Knife.h
//
//
//  Desc:   class to implement a knife
//-----------------------------------------------------------------------------
#include "Raven_Weapon.h"


class  Raven_Bot;



class Knife : public Raven_Weapon
{
public:

	Knife(Raven_Bot* owner);

	void  Render();

	void  ShootAt(Vector2D pos);

	double GetDesirability(double DistToTarget);
};



#endif
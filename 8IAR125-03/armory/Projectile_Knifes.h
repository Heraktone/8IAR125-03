#ifndef KNIFES_H
#define KNIFES_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Bolt.h
//
//  Author: Mat Buckland (ai-junkie.com)
//
//  Desc:   class to implement a bolt type projectile
//
//-----------------------------------------------------------------------------
#include "Raven_Projectile.h"

class Raven_Bot;



class Knifes : public Raven_Projectile
{
private:

	//tests the trajectory of the shell for an impact
	void TestForImpact();

public:

	Knifes(Raven_Bot* shooter, Vector2D target);

	void Render();

	void Update();

};


#endif
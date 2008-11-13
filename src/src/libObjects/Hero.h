#ifndef		_Hero_H
#define		_Hero_H

//******************************************************************

#include	"SceneObjectAnimated.h"

//******************************************************************

class Hero : public SceneObjectAnimated
{
public:
	Hero( unsigned int hpHero );

	// Gestion des HPs du héros
	unsigned int	GetHealth()						{	return health;	}
	void			SetHealth( unsigned int hp )	{	health = hp;	}
	
private:
	unsigned int health;
};

//******************************************************************
#endif		// _Hero_H

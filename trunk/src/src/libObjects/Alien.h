#ifndef		_Alien_H
#define		_Alien_H

//******************************************************************

#include	"SceneObjectAnimated.h"

//******************************************************************

class Alien : public SceneObjectAnimated
{
public:
	Alien( unsigned int hpAlien );

	// Gestion des HPs des aliens
	unsigned int	GetHealth()						{	return health;	}
	void			SetHealth( unsigned int hp )	{	health = hp;	}
	
private:
	unsigned int health;
};

//******************************************************************
#endif		// _Alien_H

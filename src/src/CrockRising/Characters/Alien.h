#ifndef		_Alien_H_
#define		_Alien_H_

//******************************************************************

#include		<vector>
#include		<Objects/SceneObjectAnimated.h>
#include		<Core/Types/Vector.h>
#include		<../CrockRising/Characters/Perso.h>
#include		<../CrockRising/Characters/Enemy.h>

//******************************************************************

class Alien : public Enemy
{
public:

	Alien() : Enemy() {}
	Alien(Vector3f position);

	virtual void			Init();
	virtual void		    changeState( PersoState newState );	
};

//******************************************************************
#endif



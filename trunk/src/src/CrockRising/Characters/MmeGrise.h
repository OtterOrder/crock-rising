#ifndef		_MmeGrise_H_
#define		_MmeGrise_H_

//******************************************************************

#include		<vector>
#include		<Objects/SceneObjectAnimated.h>
#include		<Core/Types/Vector.h>
#include		<../CrockRising/Characters/Perso.h>
#include		<../CrockRising/Characters/Enemy.h>

//******************************************************************

class MmeGrise : public Enemy
{
public:

	MmeGrise() : Enemy() {}
	MmeGrise(Vector3f position);
	
	virtual void			Init();
	virtual void		    changeState( PersoState newState );	
};

//******************************************************************
#endif



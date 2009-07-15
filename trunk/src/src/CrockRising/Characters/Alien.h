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

	Alien(Vector3f pos = Vector3f( 0.f, 0.f, 0.f ));

	virtual void	Init();
	virtual void	Hit();
	virtual void	Die();
	virtual void	InitSound();
	virtual void	InitAnim();
};

//******************************************************************
#endif



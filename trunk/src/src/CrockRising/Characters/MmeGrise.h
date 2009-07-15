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

	MmeGrise(Vector3f pos = Vector3f( 0.f, 0.f, 0.f ));

	virtual void	Init();
	virtual void	Hit();
	virtual void	Die();
	virtual void	InitSound();
	virtual void	InitAnim();
};

//******************************************************************
#endif



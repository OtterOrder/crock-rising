#ifndef		_Enemy_H_
#define		_Enemy_H_

//******************************************************************

#include		<vector>
#include		<Objects/SceneObjectAnimated.h>
#include		<Core/Types/Vector.h>
#include		<../CrockRising/Characters/Perso.h>

//******************************************************************

class Enemy : public Perso
{
public:
	static std::list<Enemy*>  RefList;
	static int nbEnemy;

	Enemy(Vector3f pos = Vector3f( 0.f, 0.f, 0.f ));

	virtual void			update();	
	virtual void			Init(){ InitSound(); InitAnim(); }
	virtual void			Hit();
	virtual void			Die();
	virtual void			DestroyPerso();
	virtual void			InitSound(){}
	virtual void			InitAnim(){}

	float timeSinceLastPath;
protected:

};

//******************************************************************
#endif



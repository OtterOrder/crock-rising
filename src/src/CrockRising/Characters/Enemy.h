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
	static std::vector<Enemy*>  listEnemy;

	Enemy();
	Enemy(Vector3f position);
	~Enemy();
	
	virtual void			  Init();
	virtual void		      changeState( PersoState newState );

	void					update( );
	SceneObjectAnimated*	getSceneObjectAnimated()	{	return m_pAnimated;	}

protected:
};

//******************************************************************
#endif



#ifndef		_Enemy_H_
#define		_Enemy_H_

//******************************************************************

#include		<vector>
#include		<Objects/SceneObjectAnimated.h>
#include		<Core/Types/Vector.h>
#include		<../CrockRising/Characters/Perso.h>
#include		<Core/Inputs/InputManager.h>

//******************************************************************

class Enemy : public Perso
{
public:
	static std::vector<Enemy*>  listEnemy;

	Enemy():Perso() {}
	Enemy(Vector3f position);
	~Enemy();

	virtual void			update( Enemy* list );	
	virtual void		    changeState( PersoState newState ){}
	virtual void			Init(){}
	virtual void			Hit();
	virtual void			Die();

protected:
	
	InputManager* m_pInputManager;
	Vector3f      m_position;
};

//******************************************************************
#endif



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

	Enemy() {}
	Enemy(Vector3f position);
	~Enemy();
	
	virtual void			Init();
	virtual void		    changeState( PersoState newState );

	void					update( );
	SceneObjectAnimated* getSceneObjectAnimated () const { return m_pAnimated; }

protected:
	InputManager* m_pInputManager;
	Vector3f      m_position;
};

//******************************************************************
#endif



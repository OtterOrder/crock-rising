#ifndef		_Hero_H_
#define		_Hero_H_

//******************************************************************

#include		<cmath>
#include		<vector>
#include		<Core/Time.h>
#include		<Core/System.h>
#include		<Core/Inputs/InputManager.h>
#include		<Physics/Physicalizer.h>
#include		<Objects/SceneObjectAnimated.h>
#include		<Objects/Camera.h>
#include		"../2D/HUDLife.h"
#include		<../CrockRising/Characters/Perso.h>


//******************************************************************

class Hero : public Perso
{
public:
	Hero();
	~Hero();

	virtual void		Init();
	virtual void		changeState( PersoState newState );
	virtual void		Hit();
	virtual void		Die();

	SceneObjectAnimated* getSceneObjectAnimated () const { return m_pAnimated; }

	//M�thodes pour g�rer l'�tat courant du H�ros
	void				update( Camera* pCamera ); 
	ResourceResult		control ( Camera* pCamera ); 

	//M�thodes li�es aux triggers (essentiellement bonus de vie pour l'instant...)
	static void			contactWithTrigger(void* param);
	static void			intoTrigger(){}
	static void			outOfTrigger(){} 
	
	SceneObject*		getArme(){ return m_pArme; }

protected:
	static HUDLife*		m_pLifeBar;
	InputManager*		m_pInputManager;
	Vector3f			m_Translate;
};

//******************************************************************
#endif



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
	Hero(Vector3f _pos = Vector3f( 0.f, 0.f, 0.f));
	~Hero();

	virtual void		Init();
	virtual void		Hit();
	virtual void		Die();
	virtual void		DestroyPerso();
	void				decLife( const int decreaseLife );
	virtual void		InitSound();
	virtual void		InitAnim();

	//Méthodes pour gérer l'état courant du Héros
	void				update(); 
	ResourceResult		control(); 

	//Méthodes liées aux triggers (essentiellement bonus de vie pour l'instant...)
	static void			contactWithTrigger(void* param);
	static void			intoTrigger(){}
	static void			outOfTrigger(){} 	

	void				setCamera(Camera* pCamera ){ m_pCamera = pCamera; }

protected:
	static HUDLife*		m_pLifeBar;
	InputManager*		m_pInputManager;
	Camera*				m_pCamera;
};

//******************************************************************
#endif



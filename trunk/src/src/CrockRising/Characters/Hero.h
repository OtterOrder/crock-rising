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


//******************************************************************

enum HeroState 
{
	STATIC,	
	WALK, 
	RUN,
	ATTACK
};


class Hero 
{
public:
	Hero();
	~Hero();

	void			  Init();

	SceneObjectAnimated* getSceneObjectAnimated () const { return m_pAnimated; }

	//Méthodes pour gérer l'état courant du Héros
	void			  update( Camera* pCamera ); 
	void		      changeState( HeroState newState );
	ResourceResult	  control ( Camera* pCamera ); 

	//Méthodes liées aux triggers (essentiellement bonus de vie pour l'instant...)
	static void		  contactWithTrigger(void* param);
	static void		  intoTrigger(){}
	static void		  outOfTrigger(){} 


protected:
	SceneObjectAnimated*	 m_pAnimated;
	static HUDLife*			 m_pLifeBar;
	HeroState				 m_currentState;
	InputManager*			 m_pInputManager;

	Vector3f				 m_Translate;
};

//******************************************************************
#endif



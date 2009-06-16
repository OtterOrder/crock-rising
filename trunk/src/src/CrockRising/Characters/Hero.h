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

//******************************************************************
typedef SceneObject Weapons;

enum HeroState 
{
	STATIC,	
	BOREDOM, //ennui
	WALK, 
	RUN,
	ATTACK,
	FLIGHT,  //fuite
	BONUS,
	FPS
};


class Hero 
{
public:
	Hero();
	~Hero();

	int				  getLife () const { return m_iLife; }
	void			  setLife ( const int life ) { m_iLife += life; }   // La variable life représentant un bonus ou malus 

	SceneObjectAnimated* getSceneObjectAnimated () const { return m_pAnimated; }

	//Méthodes pour gérer l'état courant du Héros
	void			  update( Camera* pCamera ); 
	void		      changeState( HeroState newState ) { m_currentState = newState; }
	ResourceResult	  control ( Camera* pCamera ); 

	ResourceResult			  modeFPS( Camera* pCamera );

	//Méthodes pour la gestion de l'inventaire d'armes
	bool			  addWeapon     ( Weapons* weapon);
	void			  removeWeapon  ( std::string strMesh );
	void			  OuilleOuilleOuilleCaFaitMal();


protected:
	int					     m_iLife;
	std::vector<Weapons*>    m_inventory; //Inventaire de ses armes
	SceneObjectAnimated*	 m_pAnimated;
	SceneObject*			 m_pArme;
	InputManager*			 m_pInputManager;
	HeroState				 m_currentState;
	SceneObject*             m_pWeapon;

};

//******************************************************************
#endif



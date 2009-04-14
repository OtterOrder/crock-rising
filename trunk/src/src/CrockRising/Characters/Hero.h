#ifndef		_Hero_H_
#define		_Hero_H_

//******************************************************************

#include		<Objects/SceneObjectAnimated.h>
#include		<Core/Inputs/InputManager.h>
#include		<vector>

//******************************************************************
typedef SceneObject Weapons;

class Hero 
{
public:
	Hero();
	~Hero();

	int			 getLife () const { return m_iLife; }
	void	     setLife ( const int life ) { m_iLife += life; }   // La variable life représentant un bonus ou malus 

	D3DXVECTOR3  getPosition () const { return m_position; }
	void		 setPosition ( const D3DXVECTOR3 position ) { m_position = position; }
	
	void		 control();  //Contrôle du personnage

	bool		 addWeapon     ( Weapons* weapon);
	void		 removeWeapon  ( std::string strMesh );


protected:
	int					     m_iLife;
	D3DXVECTOR3				 m_position;
	std::vector<Weapons*>    m_inventory; //Inventaire de ses armes
	
	SceneObjectAnimated*	 m_pAnimated;
	InputManager*			 m_pInputManager;

};

//******************************************************************
#endif



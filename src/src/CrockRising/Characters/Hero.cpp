#include "Hero.h"

/***********************************************************
* Constructeur
**********************************************************/
Hero::Hero()
{
	m_iLife = 100 ;
	m_position = D3DXVECTOR3(0.f,-17.f,0.f);

	m_pAnimated = new SceneObjectAnimated("Alien_Mesh.DAE","Alien_Anim.DAE",m_position);
	m_pAnimated->InitObject();
	m_pAnimated->InitDeviceData();

	m_pInputManager = InputManager::GetInstance();

	m_inventory.reserve(10);
}

/***********************************************************
* Destructeur
**********************************************************/
Hero::~Hero()
{
	delete m_pAnimated;

	m_inventory.clear();
}

/***********************************************************
* Contient les évènements claviers pour contrôler le héros  
**********************************************************/
void Hero::control( )
{
	//if ( m_pInputManager->IsKeyPressed('N') ){}
}

/***********************************************************
* Ajout d'une arme dans l'inventaire 
* In : Un SceneObject représentant une arme
* Out : Si l'inventaire est plein, retourne false sinon true
**********************************************************/
bool Hero::addWeapon(Weapons *weapon)
{
	if (m_inventory.size() < 10)
	{
		m_inventory.push_back(weapon);
		return true;
	}

	else 
		return false;
}


/*****************************************************************
* Enlève une arme de l'inventaire
* In : Nom de l'arme ( string m_strMesh de la classe SceneObject
******************************************************************/
void Hero::removeWeapon( std::string strMesh )
{
	for ( std::vector<Weapons*>::iterator it = m_inventory.begin(); it < m_inventory.end() ; it++)
	{
		if ( (*it)->getStringMesh() == strMesh )
			m_inventory.erase(it);
	}
}
#ifndef		_LevelStart_H
#define		_LevelStart_H

//******************************************************************

#include	<Game/Level.h>
#include	<Objects/Camera.h>
#include	<Renderer/Renderer.h>
#include	<Objects/Skybox.h>
#include	<Objects/SceneObjectAnimated.h>

//******************************************************************

#define		LEVEL_start				0x9f79558f

//******************************************************************

class Sprite;

class LevelStart : public Level
{
public:

	LevelStart					( crc32 levelID );			// Constructeur
	virtual ~LevelStart			( void );					// Destructeur

	virtual void	Init		( void );					// Initialisation
	virtual void	Update		( void );					// Update du niveau


protected:
	
	Camera* m_pCamera;

	SceneObjectAnimated*	m_pSObjectAnimated;
	SceneObject*			m_Alien;

	Skybox * skyb;

	Sprite	*m_pDemoRising;

	void InitInterface();	// Initialise l'interface
	void FreeInterface();	// Libère l'interface
};

//******************************************************************
#endif		//_LevelStart_H

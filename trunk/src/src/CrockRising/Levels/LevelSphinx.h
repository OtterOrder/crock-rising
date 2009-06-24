#ifndef		_LEVEL_SPHINX_H_
#define		_LEVEL_SPHINX_H_

//******************************************************************

#include	<Game/Level.h>
#include	<Objects/SceneObject.h>
#include	<Resources/Material.h>
#include	<Objects/Skybox.h>
#include	"../Characters/Hero.h"
#include	"../Characters/Enemy.h"
#include	<Renderer/Renderer.h>
#include	<Objects/Camera.h>
#include	<Physics/BoundingBoxLoader.h>
#include	<Physics/Physicalizer.h>
#include	<Objects/Light.h>
#include	<PostProcesses/PostRenderer.h>

#include	<iostream>

//******************************************************************

//faut mettre quoi?
//#define		LEVEL_canyon			0x53466a72

//******************************************************************

class Camera;

class LevelSphinx : public Level
{
public:

	LevelSphinx					( crc32 levelID );			// Constructeur
	virtual ~LevelSphinx		( void );					// Destructeur

	virtual void	Init		( void );					// Initialisation
	virtual void	Update		( void );					// Update du niveau


protected:
	
	Camera* m_pCamera;
	Hero*   m_pJanotLapin;

};

//******************************************************************
#endif		//_LEVEL_CANYON_H_

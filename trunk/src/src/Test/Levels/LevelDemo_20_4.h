#ifndef		_LevelDemo_20_4_H
#define		_LevelDemo_20_4_H

//******************************************************************

#include	<Game/Level.h>

#include	<Objects/SceneObject.h>
//#include	<Objects/SceneObjectAnimated.h>
#include	<Resources/Material.h>
#include	<Objects/Skybox.h>

#include	"../../CrockRising/Characters/Hero.h"

//******************************************************************

#define		LEVEL_demo_20_4				0xb4d3c2fd

//******************************************************************

class Camera;

class LevelDemo_20_4 : public Level
{
public:

	LevelDemo_20_4				( crc32 levelID );			// Constructeur
	virtual ~LevelDemo_20_4		( void );					// Destructeur

	virtual void	Init		( void );					// Initialisation
	virtual void	Update		( void );					// Update du niveau

protected:

	Camera* m_pCamera;
	Hero*   m_pJanotLapin;

};

//******************************************************************
#endif		//_LevelDemo_20_4_H

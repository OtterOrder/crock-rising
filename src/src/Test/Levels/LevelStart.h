#ifndef		_LevelStart_H
#define		_LevelStart_H

//******************************************************************

#include	<Game/Level.h>
#include	<Objects/SceneObject.h>
#include	<Objects/SceneObjectAnimated.h>
#include	<Objects/Skybox.h>
#include	<Resources/Material.h>

//******************************************************************

#define		LEVEL_start				0x9f79558f

//******************************************************************

class Camera;

class LevelStart : public Level
{
public:

	LevelStart					( crc32 levelID );			// Constructeur
	virtual ~LevelStart			( void );					// Destructeur

	virtual void	Init		( void );					// Initialisation
	virtual void	Update		( void );					// Update du niveau

protected:
	
	Camera* m_pCamera;

	SceneObject*			_mpCanyon;
	SceneObjectAnimated*	_mpAlien;
	Skybox*					_mpSkyBox;

	//TODO

};

//******************************************************************
#endif		//_LevelStart_H

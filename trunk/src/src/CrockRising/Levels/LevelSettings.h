#ifndef		_LEVEL_SETTINGS_H_
#define		_LEVEL_SETTINGS_H_

//******************************************************************

#include	<Game/Level.h>
#include	"../2D/ButtonText.h"

//******************************************************************

#define		LEVEL_settings			0xe545a0c5

//******************************************************************

class Camera;
class Sprite;
class Quad;
class SoundObject;
class BackButton;

class LevelSettings : public Level
{
public:

	LevelSettings				( crc32 levelID );			// Constructeur
	virtual ~LevelSettings		( void );					// Destructeur

	virtual void	Init		( void );					// Initialisation
	virtual void	Update		( void );					// Update du niveau


protected:
	
	Camera	*m_Camera;

};

//******************************************************************
#endif		//_LEVEL_SETTINGS_H_

#ifndef		_LevelStart_H
#define		_LevelStart_H

//******************************************************************

#include	<Game/Level.h>
#include	<Renderer/Renderer.h>

//******************************************************************

#define		LEVEL_start				0x9f79558f

//******************************************************************

class Sprite;
class GameCamera;

class LevelStart : public Level
{
public:

	LevelStart					( crc32 levelID );			// Constructeur
	virtual ~LevelStart			( void );					// Destructeur

	virtual void	Init		( void );					// Initialisation
	virtual void	Update		( void );					// Update du niveau


protected:
	
	GameCamera	*m_Camera;
	Sprite		*m_pSprite;

};

//******************************************************************
#endif		//_LevelStart_H

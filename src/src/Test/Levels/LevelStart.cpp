#include	"LevelStart.h"

#include	<Core/Inputs/InputManager.h>
#include	<Objects/Object2D/Sprite.h>
//#include	<XInput.h>

#include	"../Obj3d/GameCamera.h"

//******************************************************************

/***********************************************************
 * Constructeur.
 * @param[in]	levelID	: ID du niveau
 **********************************************************/
LevelStart::LevelStart( crc32 levelID )
: Level( levelID )
{
	//TODO
}

/***********************************************************
 * Destructeur.
 **********************************************************/
LevelStart::~LevelStart( void )
{
	if( m_pSprite )
		delete m_pSprite;
}

/***********************************************************
* Initialisation du niveau.
**********************************************************/
void LevelStart::Init( void )
{
	m_Camera = new GameCamera( Vector3f(0.0f, 10.0f, -100.0f) );
	m_Camera->SetTarget( Vector3f(10.0f, 0.0f, 10.0f ) );
	
	Renderer::GetInstance()->SetCamera( m_Camera );

	m_pSprite = new Sprite( 0 );
}

/***********************************************************
 * Update du niveau.
 **********************************************************/
void LevelStart::Update( void )
{
	//Partie Manette Xbox360
	/*XInputEnable( 1 );
	XINPUT_STATE stateManette = {0};
	XInputGetState( 1, &stateManette );*/

	//TODO
}

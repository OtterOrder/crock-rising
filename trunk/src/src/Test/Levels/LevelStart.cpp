#include	"LevelStart.h"

#include	<Renderer/Renderer.h>
#include	<Objects/Camera.h>

//******************************************************************

/***********************************************************
 * Constructeur.
 * @param[in]	levelID	: ID du niveau
 **********************************************************/
LevelStart::LevelStart( crc32 levelID )
: Level( levelID )
{
	m_pCamera = NULL;

	_mpCanyon	= NULL;
	_mpAlien	= NULL;
	_mpSkyBox	= NULL;
}

/***********************************************************
 * Destructeur.
 **********************************************************/
LevelStart::~LevelStart( void )
{
	if( m_pCamera )
		delete m_pCamera;
}

/***********************************************************
* Initialisation du niveau.
**********************************************************/
void LevelStart::Init( void )
{
	m_pCamera = new Camera( Vector3f( 0.0f, -10.0f, -30.0f ) );
	Renderer::GetInstance()->SetCamera( m_pCamera );

	_mpCanyon = new SceneObject("canyon1.DAE", Vector3f(0.f, 0.f, 0.f));
	_mpCanyon->InitObject();
	_mpCanyon->GetMaterial()->SetTexture("roche.jpg", Texture::DIFFUSE);

	_mpAlien = new SceneObjectAnimated("Alien_Mesh.DAE", "Alien_Anim.DAE", Vector3f(0.f, 0.f, 0.f));
	_mpAlien->InitObject();
	_mpAlien->GetMaterial()->m_AmbientColor = Vector4f(0.f, 0.2f, 0.f, 1.f);
	_mpAlien->GetMaterial()->m_DiffuseColor = Vector4f(0.f, 0.6f, 0.f, 1.f);
}

/***********************************************************
 * Update du niveau.
 **********************************************************/
void LevelStart::Update( void )
{
	//TODO
}

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

	m_pCanyon	= NULL;
	m_pAlien	= NULL;

	m_pSkybox	= NULL;
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

	m_pCanyon = new SceneObject("canyon1.DAE", Vector3f(0.f, 0.f, 0.f));
	m_pCanyon->InitObject();
	m_pCanyon->GetMaterial()->SetTexture("roche.jpg", Texture::DIFFUSE);

	m_pAlien = new SceneObjectAnimated("Alien_Mesh.DAE", "Alien_Anim.DAE", Vector3f(0.f, 0.f, 0.f));
	m_pAlien->InitObject();
	m_pAlien->GetMaterial()->m_AmbientColor = Vector4f(0.f, 0.2f, 0.f, 1.f);
	m_pAlien->GetMaterial()->m_DiffuseColor = Vector4f(0.f, 0.6f, 0.f, 1.f);

	m_pSkybox = new Skybox("mars.dds");
	m_pSkybox->Init();

	Renderer::GetInstance()->SetSkybox(m_pSkybox);
}

/***********************************************************
 * Update du niveau.
 **********************************************************/
void LevelStart::Update( void )
{
	//TODO
}

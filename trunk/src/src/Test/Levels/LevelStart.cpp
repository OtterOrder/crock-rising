#include	"LevelStart.h"

#include	<Renderer/Renderer.h>
#include	<Objects/Camera.h>
#include	<Objects/Skybox.h>
#include	<Objects/SceneObject.h>
#include	<Objects/SceneObjectAnimated.h>
#include	<Objects/Light.h>
#include    <Resources/Material.h>
#include	<PostProcesses/PostRenderer.h>

//******************************************************************

/***********************************************************
 * Constructeur.
 * @param[in]	levelID	: ID du niveau
 **********************************************************/
LevelStart::LevelStart( crc32 levelID )
: Level( levelID )
{
	m_pCamera = NULL;
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
	m_pCamera = new Camera( Vector3f( 0.0f, 10.0f, 0.0f ) );
	Renderer::GetInstance()->SetCamera( m_pCamera );

	Skybox * s=new Skybox("plainsky.dds");
	s->Init();
	Renderer::GetInstance()->SetSkybox(s);

	/*SceneObject * o=new SceneObject("canyon.DAE", D3DXVECTOR3(0.f, 0.f, 0.f));
	o->InitObject();
	o->GetMaterial()->SetTexture("roche.jpg", Texture::DIFFUSE);*/

	SceneObject * o1=new SceneObject("boite.DAE", D3DXVECTOR3(0.f, 0.f, 0.f));
	o1->Init();


	//SceneObjectAnimated * o2=new SceneObjectAnimated("alien.DAE", "Alien_anim.DAE", D3DXVECTOR3(0.f, 0.f, 0.f));
	//o2->InitObject();
	//o2->SetAnim("AnimFrappeDroite.DAE");
	
	//SceneObject * o1=new SceneObject("alien1.DAE", D3DXVECTOR3(0.f, 0.f, 0.f));
	//o1->InitObject();

	Light * DefaultLight=new SpotLight();

	PostRenderer::GetInstance()->EnablePostEffect(PostRenderer::PE_MotionBlur);



	//Renderer::GetInstance()->SetShadowMap(DefaultLight);


}

/***********************************************************
 * Update du niveau.
 **********************************************************/
void LevelStart::Update( void )
{
	//TODO
}

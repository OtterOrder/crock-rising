#include	"LevelStart.h"

#include	<Renderer/Renderer.h>
#include	<Objects/Camera.h>
#include	<Objects/SceneObject.h>
#include	<Objects/SceneObjectAnimated.h>
#include	<Core/Inputs/InputManager.h>
#include	<Objects/Light.h>
#include	<../CrockRising/Characters/Hero.h>
#include	<Physics/Physicalizer.h>

//******************************************************************

Hero* Robot = NULL;
int angle = 0;
float h = 40.f;
SceneObject* CubeKiRox = NULL;

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
	m_pCamera = new Camera(Vector3f(0.f, 15.f, -50.f));
	Renderer::GetInstance()->SetCamera( m_pCamera );

	SceneObject* Plan = new SceneObject("SolM.DAE", Vector3f(0.f, 0.f, 0.f));
	Plan->Init();
	Plan->SetObjectPhysical("SolP.DAE", GROUP_STATIC);
	
	DirectionalLight* light = new DirectionalLight();

	Robot = new Hero();
	//Robot = new SceneObjectAnimated("Alien_Mesh.DAE", "Alien_Anim.DAE", Vector3f(0.f, 1.f, 0.f));
	//Robot->Init();
	
	/*CubeKiRox = new SceneObject("CubeM.DAE", Vector3f( 0.f, 15.f, 10.f));
	CubeKiRox->Init();
	CubeKiRox->SetShader("blinn.fx");
	CubeKiRox->SetObjectPhysical("CubeP.DAE");*/
}

/***********************************************************
 * Update du niveau.
 **********************************************************/
void LevelStart::Update( void )
{
	InputManager* pInputManager = InputManager::GetInstance();
	
	//Mouvement de l'objet
	/*if( pInputManager->IsKeyTriggered( 'T' ) )
	{	
	}*/
		if(CubeKiRox)
		{
			CubeKiRox->SetTranslation(0.f, h, 0.f);
			h-=0.001f;
		}
	
	//Mouvement de l'objet
	if( pInputManager->IsKeyPressed( ' ' ) )
	{	
		angle = angle < 360 ? angle+10 : 0;
		if(CubeKiRox)
			CubeKiRox->SetRotation(0, angle, 0);
	}

/*	if( pInputManager->IsKeyPressed( 'Y' ) )
	{	
		for(int i = 0; i < 10; i++)
		{
			for(int j = 0; j < 10; j++)
			{
				SceneObject* Cube = new SceneObject("CubeM.DAE", Vector3f( i*2 + j/2, j*2+1.f, 10.f));
				Cube->Init();
				Cube->SetShader("blinn.fx");
				Cube->SetObjectPhysical("CubeP.DAE");
			}
		}			
	}*/

	if ( pInputManager->IsKeyTriggered('I'))
	{
		CubeKiRox = new SceneObject("CubeM.DAE", Vector3f( 0.f, h, 0.f));
		CubeKiRox->Init();
		CubeKiRox->SetShader("blinn.fx");
		CubeKiRox->SetObjectPhysical("CubeP.DAE", GROUP_WEAPON);

		physX::Link(Robot->getSceneObjectAnimated(), CubeKiRox);
	}

	Robot->update(m_pCamera);
}

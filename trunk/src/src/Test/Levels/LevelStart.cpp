#include	"LevelStart.h"

#include	<Renderer/Renderer.h>
#include	<Objects/Camera.h>
#include	<Objects/SceneObject.h>
#include	<Objects/SceneObjectAnimated.h>
#include	<Core/Inputs/InputManager.h>
#include	<Objects/Light.h>
#include	<Resources/Material.h>
#include	<../CrockRising/Characters/Hero.h>
#include	<Physics/Physicalizer.h>

//******************************************************************

Hero* Robot = NULL;
int angle = 0;
float h = 40.f;
SceneObject* CubeKiRox = NULL;
CFirstPersonCamera* DevCam;

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
void a()
{
	SceneObject* Cube = new SceneObject("Cube.DAE", Vector3f(0.f, 10.f, 10.f));
	Cube->Init();
	Cube->SetShader("blinn.fx");
	Cube->SetObjectPhysical("CubeP.DAE", GROUP_DYNAMIQUE);
}

/***********************************************************
* Initialisation du niveau.
**********************************************************/
void LevelStart::Init( void )
{
	m_pCamera = new Camera(Vector3f(0.f, 15.f, -100.f));
	Renderer::GetInstance()->SetCamera( m_pCamera );
	//DevCam = &Renderer::GetInstance()->m_DevCamera;

	SceneObject* Plan = new SceneObject("EscalierM.DAE", Vector3f(0.f, 0.f, 0.f));
	Plan->Init();
	Plan->SetObjectPhysical("EscalierP.DAE", GROUP_STATIC);
// 	SceneObject* Plan = new SceneObject("PenteM.DAE", Vector3f(0.f, 0.f, 0.f));
// 	Plan->Init();
// 	Plan->SetObjectPhysical("PenteP.DAE", GROUP_STATIC);

	DirectionalLight* light = new DirectionalLight();

// 	SceneObject* alien = new  SceneObject("Alien_Mesh_UV.DAE",D3DXVECTOR3(0.f,15.f,0.f));
// 	alien->Init();
// 	alien->GetMaterial()->SetTexture("alien.jpg",Texture::DIFFUSE);
// 	alien->GetMaterial()->SetTexture("alien_normal.png",Texture::NORMALMAP);
// 	alien->SetShader("default_normalmap.fx");
	SceneObject *trigger = new SceneObject("Cube.DAE", Vector3f(0.f, 5.f, 0.f));
	trigger->Init();
	trigger->SetShader("blinn.fx");
	trigger->SetObjectTrigger("CubeP.DAE", a, a, a);

	Robot = new Hero();
}

/***********************************************************
* Update du niveau.
**********************************************************/
void LevelStart::Update( void )
{
	InputManager* pInputManager = InputManager::GetInstance();


	if( pInputManager->IsKeyTriggered( ' ' ) )
	{	
	//	for(int i = 0; i < 10; i++)
	//	{
	//		for(int j = 0; j < 10; j++)
	//		{
				//SceneObject* Cube = new SceneObject("Cube.DAE", Vector3f( i*2 + j/2, j*2+1.f, 10.f));
				SceneObject* Cube = new SceneObject("Cube.DAE", Vector3f( 0.f, 10.f, 0.f));
				Cube->Init();
				Cube->SetShader("blinn.fx");
				Cube->SetObjectPhysical("CubeP.DAE", GROUP_DYNAMIQUE);
	//		}
	//	}			
	}

	if( pInputManager->IsKeyTriggered( 'T' ) )
	{	
		//Camera cam = Renderer::GetInstance()->get
		SceneObject* Cube = new SceneObject("Cube.DAE", Vector3f( m_pCamera->GetPosition().x, m_pCamera->GetPosition().y, m_pCamera->GetPosition().z));
		Cube->Init();
		Cube->SetShader("blinn.fx");
		Cube->SetObjectPhysical("CubeP.DAE", GROUP_DYNAMIQUE);
		Vector3f Dx(m_pCamera->GetTarget().x - m_pCamera->GetPosition().x,
					m_pCamera->GetTarget().y - m_pCamera->GetPosition().y,
					m_pCamera->GetTarget().z - m_pCamera->GetPosition().z);

		NxVec3 D(Dx.x, Dx.y, Dx.z);
		D.normalize();
		D *= 100;
		physX::getActor(Cube->getEmpActor())->setLinearVelocity(D);
	}

	Robot->update(m_pCamera);
}

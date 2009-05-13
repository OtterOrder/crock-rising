#include	"LevelDemo_20_4.h"

#include	<Renderer/Renderer.h>
#include	<Objects/Camera.h>
#include	<Objects/Skybox.h>
#include	<Physics/BoundingBoxLoader.h>
#include	<Objects/Light.h>
#include	<PostProcesses/PostRenderer.h>


//******************************************************************

/***********************************************************
* Constructeur.
* @param[in]	levelID	: ID du niveau
**********************************************************/
LevelDemo_20_4::LevelDemo_20_4( crc32 levelID )
: Level( levelID )
{
	m_pCamera = NULL;
	m_pJanotLapin = NULL;
}

/***********************************************************
* Destructeur.
**********************************************************/
LevelDemo_20_4::~LevelDemo_20_4( void )
{
	if( m_pCamera )
		delete m_pCamera;
}

/***********************************************************
* Initialisation du niveau.
**********************************************************/
void LevelDemo_20_4::Init( void )
{

	m_pJanotLapin = new Hero();

	Vector3f posCamInitiale = m_pJanotLapin->getSceneObjectAnimated()->GetPosition();
	posCamInitiale.y = 10.f;
	posCamInitiale.z = -40.f;
	m_pCamera = new Camera( posCamInitiale );
	float ratio = (float)Renderer::GetInstance()->GetWindowWidth() / Renderer::GetInstance()->GetWindowHeight();
	m_pCamera->SetRatio( ratio );

	Renderer::GetInstance()->SetCamera( m_pCamera );

	Skybox * s=new Skybox("mars.dds");
	s->Init();
	Renderer::GetInstance()->SetSkybox(s);

	SceneObject* robot = new SceneObject("Robot_Mesh.DAE",D3DXVECTOR3(-30.f,63.f,100.f));
	robot->InitObject();

	SceneObject * obj0=new SceneObject("canyon1.DAE", D3DXVECTOR3(0.f, -57.f, 0.f));
	obj0->InitObject();
	obj0->GetMaterial()->SetTexture("roche.jpg", Texture::DIFFUSE);
	obj0->GetMaterial()->SetTexture("roche-normal.jpg", Texture::NORMALMAP);


	DirectionalLight* dirLight = new DirectionalLight();

	PostRenderer::GetInstance()->EnablePostEffect(PostRenderer::PE_MotionBlur);

	/*SceneObject* robot = new SceneObject("Robot.DAE",D3DXVECTOR3(0.f,63.f,0.f));
	robot->InitObject();
	robot->SetRotation(0,0,90);*/

	/*BoundingBoxLoader test;
	test.Load("testPhysX.DAE");
	std::vector<DynamicBody*> testD = test.getvDynamicBody();
	std::vector<StaticBody*> testS = test.getvStaticBody();*/

	for(int i=0; i<5; i++)
	{
		SceneObjectAnimated * obj=new SceneObjectAnimated("Alien_Mesh.DAE", "animBoredom.DAE", D3DXVECTOR3(0.f, -17.f, i*10.f));
		obj->InitObject();
		obj->InitDeviceData();
		obj->GetMaterial()->m_Glossiness=80.f;
		obj->SetAnimFPS(20.f);
	}

	for(int i=0; i<5; i++)
	{
		SceneObjectAnimated * obj=new SceneObjectAnimated("Alien_Mesh.DAE", "animBoredom2.DAE", D3DXVECTOR3(-200.f, -17.f, i*10.f));
		obj->InitObject();
		obj->InitDeviceData();
		obj->GetMaterial()->m_Glossiness=80.f;
		obj->SetAnimFPS(20.f);
	}

	for(int i=0; i<5; i++)
	{
		SceneObjectAnimated * obj=new SceneObjectAnimated("Alien_Mesh.DAE", "AnimFrappeDroite.DAE", D3DXVECTOR3(-100.f, -17.f, i*10.f));
		obj->InitObject();
		obj->InitDeviceData();
		obj->GetMaterial()->m_Glossiness=80.f;
		obj->SetAnimFPS(20.f);
	}

}

/***********************************************************
* Update du niveau.
**********************************************************/
void LevelDemo_20_4::Update( void )
{
	m_pJanotLapin->update(m_pCamera);
}

#include	"LevelCanyon.h"

#include	<Renderer/Renderer.h>
#include	<Objects/Camera.h>
#include	<Physics/Trigger/UserData.h>
#include	<../CrockRising/Characters/Alien.h>
#include	<../CrockRising/Characters/MmeGrise.h>

SceneObject* Canyon = NULL;	

/***********************************************************
 * Constructeur.
 * @param[in]	levelID	: ID du niveau
 **********************************************************/
LevelCanyon::LevelCanyon( crc32 levelID )
: Level( levelID )
{
	m_pCamera = NULL;
	m_pHero = new Hero( Vector3f( 657.f, 8.f, -451.f) );
	m_EscMenu = new EscMenu();
}

/***********************************************************
 * Destructeur.
 **********************************************************/
LevelCanyon::~LevelCanyon( void )
{
	if( m_pCamera )
		delete m_pCamera;

	if(m_EscMenu)
		delete m_EscMenu;
	
	if(Canyon) 
	{
		SceneObject::RefList.remove( Canyon );
		delete Canyon;
		Canyon= NULL;
	}

	if(m_pHero)
		delete m_pHero;
	m_pHero = NULL;
}


/***********************************************************
* Initialisation du niveau.
**********************************************************/
void LevelCanyon::Init( void )
{
	Renderer *pRenderer = Renderer::GetInstance();

	// Initialisation de la caméra
	m_pCamera = new Camera(Vector3f(0.f,14.f,-60.f));
	float ratio = (float)pRenderer->GetWindowWidth() / pRenderer->GetWindowHeight();
	m_pCamera->SetRatio( ratio );
	m_pCamera->SetZNearFar(2.f, 6000.f);
	pRenderer->SetCamera(m_pCamera);

	m_EscMenu->Init();

	InputManager::GetInstance()->HoldMouseAtCenter( true );
	InputManager::GetInstance()->ShowOSCursor( false );

	//Lumière
	DirectionalLight* spot = new SpotLight();
	spot->SetLightPosition(D3DXVECTOR3(0.f, 2000.f, 20.f));
	spot->SetLightAngle(2.1f);

	//Skybox
	m_pSkybox=new Skybox("mars.dds");
	m_pSkybox->Init(35);
	pRenderer->SetSkybox(m_pSkybox);

////DESERT Mesh_CanyonFinal Physic_Canyon
	Canyon = new SceneObject("Mesh_CanyonFinal.DAE", D3DXVECTOR3(0.f, 0.f,0.f));
	Canyon->Init();
	Canyon->GetMaterial()->SetTexture("canyon_2diffuse.jpg", Texture::DIFFUSE);
	Canyon->GetMaterial()->SetTexture("normal_map_canyon.jpg", Texture::NORMALMAP);
	Canyon->GetMaterial()->m_SpecularColor=D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.f);
	Canyon->SetObjectPhysical( "Physic_Canyon.DAE" );

	//Initialisation du Héro
	m_pHero->Init();
	m_pHero->setCamera( m_pCamera );

	//std::vector<Vector3f> triggers;
	//triggers.reserve(7);
	//triggers.push_back(Vector3f(-769.f, 0.f, 442.f));
	//triggers.push_back(Vector3f(0.f, 0.f, 435.f));
	//triggers.push_back(Vector3f(668.f, 0.f, -1129.f));
	//triggers.push_back(Vector3f(-234.f, 0.f, -497.f));
	//triggers.push_back(Vector3f(0.f, 0.f, -1437.f));

	//for (size_t i = 0 ; i < triggers.size() ; ++i)
	//{
	//	SceneObjectAnimated* testTrigger = new SceneObjectAnimated("Life_Mesh.DAE","Life_Anim.DAE",triggers[i]);
	//	testTrigger->Init();
	//	testTrigger->GetMaterial()->SetTexture("crock-rising.png",Texture::DIFFUSE);
	//	testTrigger->SetObjectTrigger("Life_MeshP.DAE", Hero::contactWithTrigger, NULL,NULL, testTrigger,NULL,NULL);
	//	testTrigger->SetLoop( true );
	//	testTrigger->SetAnimFPS(30.f);
	//}
}

/***********************************************************
 * Update du niveau.
 **********************************************************/
void LevelCanyon::Update( void )
{
	m_pHero->update();
}

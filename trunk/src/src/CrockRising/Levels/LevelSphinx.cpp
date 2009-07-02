#include	"LevelSphinx.h"

#include	<Renderer/Renderer.h>
#include	<Objects/Camera.h>
#include	<Physics/Trigger/UserData.h>
#include	<../CrockRising/Characters/Alien.h>
#include	<../CrockRising/Characters/MmeGrise.h>


ContactReportCR*  gContactReportCR = new ContactReportCR;
SceneObject* cube = NULL;

void createStack(Vector3f pos)
{
	for(int i = 0; i < 15; i ++)
	{
		for(int j = 0; j < 20 - i; j++)
		{
			SceneObject* plop = new SceneObject( "CubeM.DAE", pos + Vector3f( j*1.001f - 2.5f + i/2.f, (i)*1.001f + 0.5f, 50.f));
			plop->Init();
			plop->SetObjectPhysical("CubeP.DAE");
		}
	}
}
/***********************************************************
 * Constructeur.
 * @param[in]	levelID	: ID du niveau
 **********************************************************/
LevelSphinx::LevelSphinx( crc32 levelID )
: Level( levelID )
{
	m_pCamera = NULL;
	m_pHero = new Hero();
	m_EscMenu = new EscMenu();
	
	Physicalizer::GetInstance()->setControllerCallback( gContactReportCR );
}

/***********************************************************
 * Destructeur.
 **********************************************************/
LevelSphinx::~LevelSphinx( void )
{
	if( m_pCamera )
		delete m_pCamera;

	if(m_EscMenu)
		delete m_EscMenu;
}


/***********************************************************
* Initialisation du niveau.
**********************************************************/
void LevelSphinx::Init( void )
{
	Renderer *pRenderer = Renderer::GetInstance();

	// Initialisation de la cam�ra
	m_pCamera = new Camera(Vector3f(0.f,14.f,-60.f));
	float ratio = (float)pRenderer->GetWindowWidth() / pRenderer->GetWindowHeight();
	m_pCamera->SetRatio( ratio );
	m_pCamera->SetZNearFar(2.f, 6000.f);
	pRenderer->SetCamera(m_pCamera);

	m_EscMenu->Init();

	InputManager::GetInstance()->HoldMouseAtCenter( true );
	InputManager::GetInstance()->ShowOSCursor( false );

	//Lumi�re
	DirectionalLight* spot = new SpotLight();
	spot->SetLightPosition(D3DXVECTOR3(0.f, 2000.f, 20.f));
	spot->SetLightAngle(2.1f);

	//Skybox
	m_pSkybox=new Skybox("redday.dds");
	m_pSkybox->Init(35);
	pRenderer->SetSkybox(m_pSkybox);
/*
	SceneObject* m_pCanyon_part1 = new SceneObject("planAI.DAE", D3DXVECTOR3(0.f, 1.0f, 0.f) );
	m_pCanyon_part1->Init();
	m_pCanyon_part1->GetMaterial()->SetTexture("levelSphynx.bmp", Texture::DIFFUSE );
*/
//
////DESERT Mesh_CanyonFinal Physic_Canyon
	SceneObject* Desert = new SceneObject("Mesh_Desert.DAE", D3DXVECTOR3(0.f, 0.f,0.f));
	Desert->Init();
	Desert->GetMaterial()->SetTexture("desert_diffuse.jpg", Texture::DIFFUSE);
	Desert->SetObjectPhysical( "Physic_Desert.DAE" );
	
////STATUES
	SceneObject* statue = new SceneObject("Mesh_Statue.DAE", D3DXVECTOR3(-452.4f, 0.f, 526.5f));
	statue->Init();
	statue->GetMaterial()->SetTexture("statue_diffuse.jpg", Texture::DIFFUSE);
	statue->GetMaterial()->SetTexture("statue_normal.jpg", Texture::NORMALMAP);
	statue->SetShader("default_normalmap.fx");
	statue->SetObjectPhysical("Physic_Statue.DAE");
	statue->SetRotation(0.f, 140.f, 0.f);
	
	SceneObject* statue2 = new SceneObject("Mesh_Statue.DAE", D3DXVECTOR3(452.4f, 0.f, 526.5f));
	statue2->Init();
	statue2->SetRotation(0.f, -70.f, 0.f);
	statue2->GetMaterial()->SetTexture("statue_diffuse.jpg", Texture::DIFFUSE);
	statue2->GetMaterial()->SetTexture("statue_normal.jpg", Texture::NORMALMAP);
	statue2->SetShader("default_normalmap.fx");
	//statue2->SetObjectPhysical("Physic_Statue.DAE");
	
	SceneObject* statue3 = new SceneObject("Mesh_Statue.DAE", D3DXVECTOR3(816.5f, 0.f, -984.f));
	statue3->Init();
	statue3->GetMaterial()->SetTexture("statue_diffuse.jpg", Texture::DIFFUSE);
	statue3->GetMaterial()->SetTexture("statue_normal.jpg", Texture::NORMALMAP);
	statue3->SetShader("default_normalmap.fx");
	statue3->SetObjectPhysical("Physic_Statue.DAE");
	
////PALMIERS
	SceneObject* palmier = new SceneObject("Mesh_Palmier.DAE", D3DXVECTOR3(-808.8f, 0.f,401.7f));
	palmier->Init();
	palmier->GetMaterial()->SetTexture("palmier_diffuse.jpg", Texture::DIFFUSE);
	palmier->SetObjectPhysical("Physic_Palmier.DAE");
	
	SceneObject* palmier2 = new SceneObject("Mesh_Palmier.DAE", D3DXVECTOR3(-684.5f, 0.f, 480.f));
	palmier2->Init();
	palmier2->GetMaterial()->SetTexture("palmier_diffuse.jpg", Texture::DIFFUSE);
	palmier2->SetObjectPhysical("Physic_Palmier.DAE");

	SceneObject* palmier3 = new SceneObject("Mesh_Palmier.DAE", D3DXVECTOR3(747.5f, 0.f, -1045.5f));
	palmier3->Init();
	palmier3->GetMaterial()->SetTexture("palmier_diffuse.jpg", Texture::DIFFUSE);
	palmier3->SetObjectPhysical("Physic_Palmier.DAE");

////PYRAMIDE

	SceneObject* Pyramide2 = new SceneObject("Mesh_Pyramide.DAE", D3DXVECTOR3(-519.8f, 0.f, -1181.3f));
	Pyramide2->Init();
	Pyramide2->GetMaterial()->SetTexture("pyramide_diffuse.jpg", Texture::DIFFUSE);
	Pyramide2->GetMaterial()->SetTexture("pyramide_normal.jpg", Texture::NORMALMAP);
	Pyramide2->SetShader("default_normalmap.fx");
	Pyramide2->SetObjectPhysical("Physic_Pyramide.DAE");

////SPHINX
 	SceneObject* sphinx = new SceneObject("Mesh_Sphinx.DAE", D3DXVECTOR3(-41.5f, 0.f, 921.5f));
 	sphinx->Init();
 	sphinx->GetMaterial()->SetTexture("sphinx_diffuse.jpg", Texture::DIFFUSE);
 	sphinx->GetMaterial()->SetTexture("sphinx_normal.jpg", Texture::NORMALMAP);
 	sphinx->SetShader("default_normalmap.fx");
	sphinx->SetObjectPhysical("Physic_Sphinx.DAE");

	//Initialisation du H�ro
	m_pHero->Init();

	createStack( Vector3f(0.f, 0.f, -200.f) );
	createStack( Vector3f(245.f, 0.f, 200.f) );
	createStack( Vector3f(-100.f, 0.f, -400.f) );
	createStack( Vector3f(400.f, 0.f, -200.f) );
	createStack( Vector3f(-500.f, 0.f, -700.f) );


	// Cr�ation de l'AI
	m_pManagerAI = new AIManager( true, AIManager::AI_NORMAL, 5, 400, 30, 3100, 256 );
}

/***********************************************************
 * Update du niveau.
 **********************************************************/
void LevelSphinx::Update( void )
{
	// Mise a jour de l'AI avec les ennemies en param�tre
	deltaTime = System::GetInstance()->GetTime()->GetDeltaTimeMs();
	Vector3f posHero = m_pHero->getSceneObjectAnimated()->GetPosition();
	m_pManagerAI->update( m_pHero, deltaTime, m_pEnemy[0]->listEnemy );

	m_pHero->update(m_pCamera);
}

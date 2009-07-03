#include	"LevelSphinx.h"

#include	<Renderer/Renderer.h>
#include	<Objects/Camera.h>
#include	<Physics/Trigger/UserData.h>
#include	<../CrockRising/Characters/Alien.h>
#include	<../CrockRising/Characters/MmeGrise.h>

SceneObject* Desert = NULL;	
SceneObject* statue = NULL;	
SceneObject* statue2 = NULL;	
SceneObject* statue3 = NULL;	
SceneObject* palmier = NULL;	
SceneObject* palmier2 = NULL;
SceneObject* palmier3 = NULL;
SceneObject* Pyramide2 = NULL;
SceneObject* sphinx = NULL;
std::vector<Vector3f> triggersPos;
std::vector<SceneObjectAnimated*> triggers;

ContactReportCR*  gContactReportCR = new ContactReportCR;

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
	m_pHero = new Hero(Vector3f( 0.f, 8.f, 0.f) );
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
	
	if(Desert) 
	{
		SceneObject::RefList.remove( Desert );
		delete Desert;
		Desert= NULL;
	}
	
	if(statue) 
	{
		SceneObject::RefList.remove( statue );
		delete statue;
		statue= NULL;
	}
	
	if(statue2) 
	{
		SceneObject::RefList.remove( statue2 );
		delete statue2;
		statue2 = NULL;
	}
	
	if(statue3) 
	{
		SceneObject::RefList.remove( statue3 );
		delete statue3;
		statue3 = NULL;
	}
	
	if(palmier) 
	{
		SceneObject::RefList.remove( palmier );
		delete palmier;
		palmier = NULL;
	}
	
	if(palmier2) 
	{
		SceneObject::RefList.remove( palmier2 );
		delete palmier2;
		palmier2 = NULL;
	}
	
	if(palmier3) 
	{
		SceneObject::RefList.remove( palmier3 );
		delete palmier3;
		palmier3 = NULL;
	}
	
	if(Pyramide2) 
	{
		SceneObject::RefList.remove( Pyramide2 );
		delete Pyramide2;
		Pyramide2 = NULL;
	}
 	
	if(sphinx) 
	{
		SceneObject::RefList.remove( sphinx );
		delete sphinx;
		sphinx = NULL;
	}

	if(m_pManagerAI) 
		delete m_pManagerAI;
	m_pManagerAI = NULL;

	if(m_pHero)
		delete m_pHero;
	m_pHero = NULL;

	for (size_t i = 0 ; i < triggers.size() ; ++i)
	{		
		if(triggers[i]) 
		{
			SceneObject::RefList.remove( triggers[i] );
		}
	}
}


/***********************************************************
* Initialisation du niveau.
**********************************************************/
void LevelSphinx::Init( void )
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
	m_pSkybox=new Skybox("redday.dds");
	m_pSkybox->Init(35);
	pRenderer->SetSkybox(m_pSkybox);

////DESERT Mesh_CanyonFinal Physic_Canyon
	Desert = new SceneObject("Mesh_Desert.DAE", D3DXVECTOR3(0.f, 0.f,0.f));
	Desert->Init();
	Desert->GetMaterial()->SetTexture("desert_diffuse.jpg", Texture::DIFFUSE);
	Desert->GetMaterial()->m_SpecularColor=D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.f);
	Desert->SetObjectPhysical( "Physic_Desert.DAE" );
	
////STATUES
	statue = new SceneObject("Mesh_Statue.DAE", D3DXVECTOR3(-452.4f, 0.f, 526.5f));
	statue->Init();
	statue->GetMaterial()->SetTexture("statue_diffuse.jpg", Texture::DIFFUSE);
	statue->GetMaterial()->SetTexture("statue_normal.jpg", Texture::NORMALMAP);
	statue->SetShader("default_normalmap.fx");
	statue->SetObjectPhysical("Physic_Statue.DAE");
	statue->SetRotation(0.f, 140.f, 0.f);
	
	statue2 = new SceneObject("Mesh_Statue.DAE", D3DXVECTOR3(452.4f, 0.f, 526.5f));
	statue2->Init();
	statue2->SetRotation(0.f, -70.f, 0.f);
	statue2->GetMaterial()->SetTexture("statue_diffuse.jpg", Texture::DIFFUSE);
	statue2->GetMaterial()->SetTexture("statue_normal.jpg", Texture::NORMALMAP);
	statue2->SetShader("default_normalmap.fx");
	//statue2->SetObjectPhysical("Physic_Statue.DAE");
	
	statue3 = new SceneObject("Mesh_Statue.DAE", D3DXVECTOR3(816.5f, 0.f, -984.f));
	statue3->Init();
	statue3->GetMaterial()->SetTexture("statue_diffuse.jpg", Texture::DIFFUSE);
	statue3->GetMaterial()->SetTexture("statue_normal.jpg", Texture::NORMALMAP);
	statue3->SetShader("default_normalmap.fx");
	statue3->SetObjectPhysical("Physic_Statue.DAE");
	
////PALMIERS
	palmier = new SceneObject("Mesh_Palmier.DAE", D3DXVECTOR3(-808.8f, 0.f,401.7f));
	palmier->Init();
	palmier->GetMaterial()->SetTexture("palmier_diffuse.jpg", Texture::DIFFUSE);
	palmier->GetMaterial()->m_SpecularColor=D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.f);
	palmier->SetObjectPhysical("Physic_Palmier.DAE");
	
	palmier2 = new SceneObject("Mesh_Palmier.DAE", D3DXVECTOR3(-684.5f, 0.f, 480.f));
	palmier2->Init();
	palmier2->GetMaterial()->SetTexture("palmier_diffuse.jpg", Texture::DIFFUSE);
	palmier2->GetMaterial()->m_SpecularColor=D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.f);
	palmier2->SetObjectPhysical("Physic_Palmier.DAE");

	palmier3 = new SceneObject("Mesh_Palmier.DAE", D3DXVECTOR3(747.5f, 0.f, -1045.5f));
	palmier3->Init();
	palmier3->GetMaterial()->SetTexture("palmier_diffuse.jpg", Texture::DIFFUSE);
	palmier3->GetMaterial()->m_SpecularColor=D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.f);
	palmier3->SetObjectPhysical("Physic_Palmier.DAE");

////PYRAMIDE

	Pyramide2 = new SceneObject("Mesh_Pyramide.DAE", D3DXVECTOR3(-519.8f, 0.f, -1181.3f));
	Pyramide2->Init();
	Pyramide2->GetMaterial()->SetTexture("pyramide_diffuse.jpg", Texture::DIFFUSE);
	Pyramide2->GetMaterial()->SetTexture("pyramide_normal.jpg", Texture::NORMALMAP);
	Pyramide2->SetShader("default_normalmap.fx");
	Pyramide2->SetObjectPhysical("Physic_Pyramide.DAE");

////SPHINX
 	sphinx = new SceneObject("Mesh_Sphinx.DAE", D3DXVECTOR3(-41.5f, 0.f, 921.5f));
 	sphinx->Init();
 	sphinx->GetMaterial()->SetTexture("sphinx_diffuse.jpg", Texture::DIFFUSE);
 	sphinx->GetMaterial()->SetTexture("sphinx_normal.jpg", Texture::NORMALMAP);
 	sphinx->SetShader("default_normalmap.fx");
	sphinx->SetObjectPhysical("Physic_Sphinx.DAE");

	//Initialisation du Héro
	m_pHero->Init();

	triggersPos.reserve(7);
	triggers.reserve(7);

	triggersPos.push_back(Vector3f(-769.f, 0.f, 442.f));
	triggersPos.push_back(Vector3f(0.f, 0.f, 435.f));
	triggersPos.push_back(Vector3f(668.f, 0.f, -1129.f));
	triggersPos.push_back(Vector3f(-234.f, 0.f, -497.f));
	triggersPos.push_back(Vector3f(0.f, 0.f, -1337.f));

	for (size_t i = 0 ; i < triggersPos.size() ; ++i)
	{
		triggers.push_back(new SceneObjectAnimated("Life_Mesh.DAE","Life_Anim.DAE",triggersPos[i]));
		triggers[i]->Init();
		triggers[i]->GetMaterial()->SetTexture("crock-rising.png",Texture::DIFFUSE);
		triggers[i]->SetObjectTrigger("Life_MeshP.DAE", Hero::contactWithTrigger, NULL,NULL, triggers[i],NULL,NULL);
		triggers[i]->SetLoop( true );
		triggers[i]->SetAnimFPS(30.f);
	}

	// Création de l'AI
	m_pManagerAI = new AIManager( true, AIManager::AI_NORMAL, 30, 400, 30, 3100, 256 );
}

/***********************************************************
 * Update du niveau.
 **********************************************************/
void LevelSphinx::Update( void )
{
	// Mise a jour de l'AI avec les ennemies en paramètre
	deltaTime = System::GetInstance()->GetTime()->GetDeltaTimeMs();
	Vector3f posHero = m_pHero->getSceneObjectAnimated()->GetPosition();
	m_pManagerAI->update( m_pHero, deltaTime, m_pEnemy[0]->listEnemy );

	m_pHero->update(m_pCamera);
}

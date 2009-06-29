#include	"LevelSphinx.h"

#include	<Renderer/Renderer.h>
#include	<Objects/Camera.h>

//******************************************************************
//Gestion des triggers

/***********************************************************
 * Constructeur.
 * @param[in]	levelID	: ID du niveau
 **********************************************************/
LevelSphinx::LevelSphinx( crc32 levelID )
: Level( levelID )
{
	m_pCamera = NULL;
	m_pJanotLapin = new Hero();
}

/***********************************************************
 * Destructeur.
 **********************************************************/
LevelSphinx::~LevelSphinx( void )
{
	if( m_pCamera )
		delete m_pCamera;
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
	m_pCamera->SetZNearFar(2.f, 3000.f);
	pRenderer->SetCamera(m_pCamera);

	//Lumière
	DirectionalLight* dirLight = new DirectionalLight();

	//Skybox
	Skybox * s=new Skybox("desert2.dds");
	s->Init();
	pRenderer->SetSkybox(s);
/*
	SceneObject* m_pCanyon_part1 = new SceneObject("planAI.DAE", D3DXVECTOR3(0.f, 1.0f, 0.f) );
	m_pCanyon_part1->Init();
	m_pCanyon_part1->GetMaterial()->SetTexture("levelSphynx.bmp", Texture::DIFFUSE );
*/
//
////DESERT
	SceneObject* Desert = new SceneObject("desert.DAE", D3DXVECTOR3(0.f,0.f,0.f));
	Desert->Init();
	Desert->GetMaterial()->SetTexture("desert_diffuse.jpg", Texture::DIFFUSE);
	Desert->SetObjectPhysical( "desert_physique.DAE"
		);
//	
////STATUES
	SceneObject* statue = new SceneObject("statue.DAE", D3DXVECTOR3(-452.4f, 0.f, 526.5f));
	statue->Init();
	statue->SetRotation(0.f, 180.f, 0.f);
	statue->GetMaterial()->SetTexture("statue_diffuse.jpg", Texture::DIFFUSE);
	statue->GetMaterial()->SetTexture("statue_normal.jpg", Texture::NORMALMAP);
	statue->SetShader("default_normalmap.fx");
	
	SceneObject* statue2 = new SceneObject("statue.DAE", D3DXVECTOR3(452.4f, 0.f, 526.5f));
	statue2->Init();
	statue2->SetRotation(0.f, 180.f, 0.f);
	statue2->GetMaterial()->SetTexture("statue_diffuse.jpg", Texture::DIFFUSE);
	statue2->GetMaterial()->SetTexture("statue_normal.jpg", Texture::NORMALMAP);
	statue2->SetShader("default_normalmap.fx");
	
	SceneObject* statue3 = new SceneObject("statue.DAE", D3DXVECTOR3(816.5f, 0.f, -984.f));
	statue3->Init();
	statue3->GetMaterial()->SetTexture("statue_diffuse.jpg", Texture::DIFFUSE);
	statue3->GetMaterial()->SetTexture("statue_normal.jpg", Texture::NORMALMAP);
	statue3->SetShader("default_normalmap.fx");
//	
////PALMIERS
	SceneObject* palmier = new SceneObject("palmier.DAE", D3DXVECTOR3(-808.8f, 0.f,401.7f));
	palmier->Init();
	palmier->GetMaterial()->SetTexture("palmier_diffuse.jpg", Texture::DIFFUSE);
	
	SceneObject* palmier2 = new SceneObject("palmier.DAE", D3DXVECTOR3(-684.5f, 0.f, 480.f));
	palmier2->Init();
	palmier2->GetMaterial()->SetTexture("palmier_diffuse.jpg", Texture::DIFFUSE);

	SceneObject* palmier3 = new SceneObject("palmier.DAE", D3DXVECTOR3(747.5f, 0.f, -1045.5f));
	palmier3->Init();
	palmier3->GetMaterial()->SetTexture("palmier_diffuse.jpg", Texture::DIFFUSE);
//
////PYRAMIDE
	SceneObject* Pyramide = new SceneObject("pyramide.DAE", D3DXVECTOR3(806.8f, 0.f, -117.767f));
	Pyramide->Init();
	Pyramide->GetMaterial()->SetTexture("pyramide_diffuse.jpg", Texture::DIFFUSE);
	Pyramide->GetMaterial()->SetTexture("pyramide_normal.jpg", Texture::NORMALMAP);
	Pyramide->SetShader("default_normalmap.fx");
	
	SceneObject* Pyramide2 = new SceneObject("pyramide.DAE", D3DXVECTOR3(-519.8f, 0.f, -1181.3f));
	Pyramide2->Init();
	Pyramide2->GetMaterial()->SetTexture("pyramide_diffuse.jpg", Texture::DIFFUSE);
	Pyramide2->GetMaterial()->SetTexture("pyramide_normal.jpg", Texture::NORMALMAP);
	Pyramide2->SetShader("default_normalmap.fx");
//
////SPHINX
	SceneObject* sphinx = new SceneObject("sphinx.DAE", D3DXVECTOR3(-41.5f, 0.f, 921.5f));
	sphinx->Init();
	sphinx->GetMaterial()->SetTexture("sphinx_diffuse.jpg", Texture::DIFFUSE);
	sphinx->GetMaterial()->SetTexture("sphinx_normal.jpg", Texture::NORMALMAP);
	sphinx->SetShader("default_normalmap.fx");

	//Initialisation du Héro
	m_pJanotLapin->Init();

	// Création de l'AI
	m_pManagerAI = new AIManager( true, AIManager::AI_NORMAL, 100, 4000, 2, 3100, 256 );

	// Création des ennemies
	for (int i=0; i<30; i++)
	{
		m_pEnemy[i] = new Enemy( Vector3f(0.f,8.f,0.f) );
	}
}

/***********************************************************
 * Update du niveau.
 **********************************************************/
void LevelSphinx::Update( void )
{
	// Mise a jour de l'AI avec les ennemies en paramètre
	deltaTime = System::GetInstance()->GetTime()->GetDeltaTimeMs();
	Vector3f posHero = m_pJanotLapin->getSceneObjectAnimated()->GetPosition();
	m_pManagerAI->update( posHero, deltaTime, m_pEnemy[0]->listEnemy );

	//std::cout << "X: " << posHero.x << "Y: " << posHero.y << "Z: " << posHero.z << std::endl;

	m_pJanotLapin->update(m_pCamera);
		
	InputManager* pInputManager = InputManager::GetInstance();

	if( pInputManager->IsKeyTriggered(' ') )
	{	
		m_pJanotLapin->getSceneObjectAnimated()->SetRotation(0.f, 1.f, 0.f);
	}
}

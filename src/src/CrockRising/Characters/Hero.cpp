#include "Hero.h"
#include <iostream>

#include	<Resources/Material.h>
#include	<../CrockRising/Characters/Perso.h>

#define MAX_LIFE   100
#define LIFE_BONUS 10

HUDLife* Hero::m_pLifeBar = NULL;

/***********************************************************
* Constructeur
**********************************************************/
Hero::Hero()
	:Perso()
{
	m_pAnimated = NULL;	
	
	m_currentState = STATIC;
	
	m_pLifeBar = new HUDLife();
	
	m_pInputManager = InputManager::GetInstance();
}

/**********************************************************
* Initialisation des données membres
**********************************************************/
void Hero::Init()
{
	m_pAnimated = new SceneObjectAnimated("Mesh_Robot.DAE","Anim_Robot_Run.DAE",D3DXVECTOR3(0.f,10.f,0.f)); //y = -75 pr le canyon
	m_pAnimated->Init();
	m_pAnimated->GetMaterial()->SetTexture("robot.png", Texture::DIFFUSE);
	m_pAnimated->GetMaterial()->SetTexture("robot_normal.dds", Texture::NORMALMAP);
	m_pAnimated->SetShader("default_skinnormalmap.fx");
	m_pAnimated->SetRotation(0.f, 180.f,0.f);
	m_pAnimated->SetControledCharacter(4.f,10.f,this);
	//m_pAnimated->SetLoop(true);
	//m_pAnimated->SetAnimFPS(50.f);

	//m_pInputManager->HoldMouseAtCenter(true);

	m_pLifeBar->Init();
	m_pLifeBar->SetMaxLife(MAX_LIFE);
	m_pLifeBar->SetLife(50);
}

/***********************************************************
* Destructeur
**********************************************************/
Hero::~Hero()
{

}


/*****************************************************************
* Suivant la touche appuyée, change l'état actuel du Héros ou bien 
* gère son déplacement. Cette méthode sert également à la mise à jour 
* de l'orientation de la caméra
******************************************************************/
ResourceResult Hero::control( Camera* pCamera )
{
	changeState(STATIC);

	//-- On vérifie tout d'abord suivant la touche tapée si l'on doit
	//   changer l'état du Héros  
	if (m_pInputManager->IsMouseTriggered(MOUSE_LEFT))
	{
		changeState(ATTACK);
		return RES_SUCCEED;
	}
	
	
	//Mise à jour de l'orientation de la caméra 
	Point2f point = m_pInputManager->GetMouseVector();
	const int sensibiliteSouris = 10;

	
	if( point.x != 0 ) 
	{
		int offsetCursor = (int)point.x%sensibiliteSouris; 
		float diff = pCamera->GetOrientationYRad();
		pCamera->SetOrientationY( -(float)offsetCursor );
		diff = pCamera->GetOrientationYRad() - diff;

		D3DXMATRIX rot;
		D3DXMatrixRotationZ( &rot, diff );
		m_pAnimated->ApplyTransform( &rot );
		//m_pAnimated->SetRotation( 0.f, diff, 0.f );
		m_pAnimated->m_vAngleY -= D3DXToDegree( diff );

	}


	if( point.y != 0 ) 
	{
		int offsetCursor = (int)point.y%sensibiliteSouris; 
		pCamera->SetOrientationX( (float)offsetCursor );
	}


	//Gestion du déplacement du Héros
	float timeF = System::GetInstance()->GetTime()->GetDeltaTimeMs();
	float vitesse = 0.09f; //[0.01; 0.4]
	float sensibilityTranslation = vitesse * timeF;

	m_Translate = Vector3f (0.f, 0.f, 0.f);

	if ( m_pInputManager->IsKeyPressed('Z'))
	{
		float xStep, zStep;
		xStep = -(std::sin(pCamera->GetOrientationYRad()))*sensibilityTranslation;
		zStep = std::cos(pCamera->GetOrientationYRad())*sensibilityTranslation;
		m_Translate += Vector3f(xStep,0.f,zStep);
		changeState(RUN);
	}
	if ( m_pInputManager->IsKeyPressed('Q'))
	{
		float xStep, zStep;
		xStep = -cos( pCamera->GetOrientationYRad() )*sensibilityTranslation;
		zStep = -sin( pCamera->GetOrientationYRad() )*sensibilityTranslation;
		m_Translate += Vector3f(xStep,0.f,zStep);
		changeState(RUN);
	}
	if ( m_pInputManager->IsKeyPressed('S'))
	{
		float xStep, zStep;
		xStep = sin( pCamera->GetOrientationYRad() )*sensibilityTranslation;
		zStep = -cos( pCamera->GetOrientationYRad() )*sensibilityTranslation;
		m_Translate += Vector3f(xStep,0.f,zStep);
		changeState(RUN);
	}
	if ( m_pInputManager->IsKeyPressed('D'))
	{
		float xStep, zStep;
		xStep = cos( pCamera->GetOrientationYRad() )*sensibilityTranslation;
		zStep = sin( pCamera->GetOrientationYRad() )*sensibilityTranslation;
		m_Translate += Vector3f(xStep,0.f,zStep);
		changeState(RUN);
	}
	

	return RES_SUCCEED;
}

/********************************************************************
* En fonction du nouvel état, cette méthode configure les nouvelles 
* animations à lancer 
*********************************************************************/
void Hero::changeState( PersoState newState )
{
	if ( (m_currentState != STATIC && !m_pAnimated->IsAtEnd())
		 || newState == m_currentState )
		return;

	m_currentState = newState;
	
	switch ( m_currentState )
	{
	case RUN :
		m_pAnimated->SetAnim("Anim_Robot_Run.DAE");
		m_pAnimated->Play();
		m_pAnimated->SetLoop(true);
		m_pAnimated->SetAnimFPS(50.f);
		break;
	case ATTACK : 
		m_pAnimated->SetAnim("Anim_Robot_Attack.DAE");
		m_pAnimated->Play();
		m_pAnimated->SetLoop(true);
		m_pAnimated->SetAnimFPS(50.f);
		break;
	case STATIC :
		m_pAnimated->Stop();
		break;
	}
}

/*********************************************************************
* Mise à jour du Héros
* Une fois les contrôles éventuels vérifiés plus les changements d'état,
* la position du Héros est modifiée
*********************************************************************/
void Hero::update( Camera* pCamera )
{
	control( pCamera );

	Vector3f pos = m_pAnimated->GetPosition();
	//std::cout << "x = " << pos.x << "  y = " << pos.y << "  z = " << pos.z << std::endl;

	if(m_currentState == RUN)
		m_pAnimated->SetTranslation(m_Translate.x, m_Translate.y, m_Translate.z);

	pCamera->SetTarget(m_pAnimated->GetPosition());
	pCamera->Update();

}

/******************************************************************************
*Si on entre en contact avec un trigger (ici une capsule donnant de la vie)
*on augmente la vie, et par la même occasion on détruit le SceneObjectAnimated
*******************************************************************************/
void Hero::contactWithTrigger(void* param)
{
	int life = m_pLifeBar->GetLife();
	if (life < MAX_LIFE)
	{
	  life += LIFE_BONUS;
	  m_pLifeBar->SetLife(life);
	  SceneObjectAnimated* capsule = (SceneObjectAnimated*)param;
	  SceneObjectAnimated::RefList.remove(capsule);
	  delete capsule;
	}
}
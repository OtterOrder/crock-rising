#include "Hero.h"

#define MAX_LIFE   100
#define LIFE_BONUS 10

HUDLife* Hero::m_pLifeBar = NULL;


/***********************************************************
* Constructeur
**********************************************************/
Hero::Hero()
{
	m_pAnimated = new SceneObjectAnimated("Robot_Mesh.DAE","Robot_Anim_Run.DAE",D3DXVECTOR3(0.f,8.f,0.f));	
	
	m_currentState = STATIC;
	
	m_pLifeBar = new HUDLife();
	
	m_pInputManager = InputManager::GetInstance();
	
}

/**********************************************************
* Initialisation des données membres
**********************************************************/
void Hero::Init()
{
	m_pAnimated->Init();
	m_pAnimated->SetRotation(0,180,0);
	m_pAnimated->SetControledCharacter(4.f,10.f,this,PHYS_HERO);

	m_pInputManager->HoldMouseAtCenter(true);

	m_pLifeBar->Init();
	m_pLifeBar->SetMaxLife(MAX_LIFE);
	m_pLifeBar->SetLife(50);
	

}

/***********************************************************
* Destructeur
**********************************************************/
Hero::~Hero()
{
	delete m_pAnimated;
}


/*****************************************************************
* Suivant la touche appuyée, change l'état actuel du Héros ou bien 
* gère son déplacement. Cette méthode sert également à la mise à jour 
* de l'orientation de la caméra
******************************************************************/
ResourceResult Hero::control( Camera* pCamera )
{
	bool isTouchPressed = false;

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
		pCamera->SetOrientationY( -offsetCursor );
		diff = pCamera->GetOrientationYRad() - diff;

		D3DXMATRIX rot;
		D3DXMatrixRotationZ( &rot, diff );
		m_pAnimated->ApplyTransform( &rot );

	}
	if( point.y != 0 ) 
	{
		int offsetCursor = (int)point.y%sensibiliteSouris; 
		pCamera->SetOrientationX( offsetCursor );
	}


	//Gestion du déplacement du Héros
	// PROBLEME = quand le FPS est trop bas, l'input manager ne réagis pas tout de suite à une pression de la touche, du coup le héro avance par à coups
	float timeF = System::GetInstance()->GetTime()->GetDeltaTimeMs();
	float vitesse = 0.09f; //[0.01; 0.4]
	float sensibilityTranslation = vitesse * timeF;

	if ( m_pInputManager->IsKeyPressed('Z'))
	{
		changeState(WALK);
		float xStep, zStep;
		xStep = -(std::sin(pCamera->GetOrientationYRad()))*sensibilityTranslation;
		zStep = std::cos(pCamera->GetOrientationYRad())*sensibilityTranslation;
		m_pAnimated->SetTranslation(xStep,0.f,zStep);
		isTouchPressed = true;
	}
	if ( m_pInputManager->IsKeyPressed('Q'))
	{
		changeState(WALK);
		float xStep, zStep;
		xStep = -cos( pCamera->GetOrientationYRad() )*sensibilityTranslation;
		zStep = -sin( pCamera->GetOrientationYRad() )*sensibilityTranslation;
		m_pAnimated->SetTranslation(xStep,0.f,zStep);
		isTouchPressed = true;
	}
	if ( m_pInputManager->IsKeyPressed('S'))
	{
		changeState(WALK);
		float xStep, zStep;
		xStep = sin( pCamera->GetOrientationYRad() )*sensibilityTranslation;
		zStep = -cos( pCamera->GetOrientationYRad() )*sensibilityTranslation;
		m_pAnimated->SetTranslation(xStep,0.f,zStep);
		isTouchPressed = true;
	}
	if ( m_pInputManager->IsKeyPressed('D'))
	{
		changeState(WALK);
		float xStep, zStep;
		xStep = cos( pCamera->GetOrientationYRad() )*sensibilityTranslation;
		zStep = sin( pCamera->GetOrientationYRad() )*sensibilityTranslation;
		m_pAnimated->SetTranslation(xStep,0.f,zStep);
		isTouchPressed = true;
	}
	if (!isTouchPressed)
		changeState(STATIC);


	return RES_SUCCEED;
}


void Hero::update( Camera* pCamera )
{
	control( pCamera );

	pCamera->SetTarget(m_pAnimated->GetPosition());

	switch ( m_currentState )
	{
	case WALK : 
		m_pAnimated->StartAnim();
		m_pAnimated->SetAnim("Robot_Anim_Run.DAE");
		m_pAnimated->SetAnimFPS(50.f);
		break;
	case RUN :
		break;
	case ATTACK : 
		m_pAnimated->StartAnim();
		m_pAnimated->SetAnim("robot_attack_anim.DAE");
		m_pAnimated->SetAnimFPS(50.f);
		break;
	case STATIC :
		m_pAnimated->StopAnim();
		break;
	}
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

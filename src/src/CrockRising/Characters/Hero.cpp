#include "Hero.h"

/***********************************************************
* Constructeur
**********************************************************/
Hero::Hero()
{
	m_iLife = 100 ;

	m_pAnimated = new SceneObjectAnimated("Alien_Mesh.DAE","Alien_Anim.DAE",D3DXVECTOR3(0.f,1.f,0.f));	
	m_pAnimated->Init();
	m_pAnimated->SetRotation(0,0,90);
	m_pAnimated->SetControledCharacter(4.f,10.f,this,GROUP_HERO);

	m_pInputManager = InputManager::GetInstance();
	m_pInputManager->HoldMouseAtCenter(true);

	m_inventory.reserve(10);
	m_currentState = WALK;

}

/***********************************************************
* Destructeur
**********************************************************/
Hero::~Hero()
{
	delete m_pAnimated;

	m_inventory.clear();
}

/***********************************************************
* Ajout d'une arme dans l'inventaire 
* In : Un SceneObject représentant une arme
* Out : Si l'inventaire est plein, retourne false sinon true
**********************************************************/
bool Hero::addWeapon(Weapons *weapon)
{
	if (m_inventory.size() < 10)
	{
		m_inventory.push_back(weapon);
		return true;
	}

	else 
		return false;
}


/*****************************************************************
* Enlève une arme de l'inventaire
* In : Nom de l'arme ( string m_strMesh de la classe SceneObject
******************************************************************/
void Hero::removeWeapon( std::string strMesh )
{
	for ( std::vector<Weapons*>::iterator it = m_inventory.begin(); it < m_inventory.end() ; it++)
	{
		if ( (*it)->getStringMesh() == strMesh )
			m_inventory.erase(it);
	}
}


/*****************************************************************
* Suivant la touche appuyée, change l'état actuel du Héros ou bien 
* gère son déplacement. Cette méthode sert également à la mise à jour 
* de l'orientation de la caméra
******************************************************************/
ResourceResult Hero::control( Camera* pCamera )
{
	//-- On vérifie tout d'abord suivant la touche tapée si l'on doit
	//   changer l'état du Héros  
	if ( m_pInputManager->IsKeyPressed(' '))
	{
		changeState(ATTACK);
		return RES_SUCCEED;
	}
	if ( m_pInputManager->IsKeyPressed('X'))
	{
		changeState(STATIC);
		return RES_SUCCEED;
	}
	if ( m_pInputManager->IsKeyPressed('V'))
	{
		changeState(BOREDOM);
		return RES_SUCCEED;
	}
	if ( m_pInputManager->IsKeyPressed('B'))
	{
		changeState(BONUS);
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
 	Time* time = System::GetInstance()->GetTime();
 	const float timeF = time->GetDeltaTimeF(); 
	const float factor = 0.5f;
	if ( m_pInputManager->IsKeyPressed('Z'))
	{
		changeState(WALK);
		float xStep, zStep;
		float sensibilityTranslation = factor*timeF;
		xStep = -(std::sin(pCamera->GetOrientationYRad()))*sensibilityTranslation;
		zStep = std::cos(pCamera->GetOrientationYRad())*sensibilityTranslation;
		m_pAnimated->SetTranslation(xStep,0.f,zStep);
	}
	else if ( m_pInputManager->IsKeyPressed('Q'))
	{
		changeState(WALK);
		float xStep, zStep;
		float sensibilityTranslation = factor*timeF;
		xStep = -cos( pCamera->GetOrientationYRad() )*sensibilityTranslation;
		zStep = -sin( pCamera->GetOrientationYRad() )*sensibilityTranslation;
		m_pAnimated->SetTranslation(xStep,0.f,zStep);
	}
	else if ( m_pInputManager->IsKeyPressed('S'))
	{
		changeState(WALK);
		float xStep, zStep;
		float sensibilityTranslation = factor*timeF;
		xStep = sin( pCamera->GetOrientationYRad() )*sensibilityTranslation;
		zStep = -cos( pCamera->GetOrientationYRad() )*sensibilityTranslation;
		m_pAnimated->SetTranslation(xStep,0.f,zStep);
	}
	else if ( m_pInputManager->IsKeyPressed('D'))
	{
		changeState(WALK);
		float xStep, zStep;
		float sensibilityTranslation = factor*timeF;
		xStep = cos( pCamera->GetOrientationYRad() )*sensibilityTranslation;
		zStep = sin( pCamera->GetOrientationYRad() )*sensibilityTranslation;
		m_pAnimated->SetTranslation(xStep,0.f,zStep);
	}
	else if ( m_pInputManager->IsKeyPressed('F'))
	{
		int angleY = int(pCamera->GetOrientationYRad() + 1.f);
		pCamera->SetOrientationY(angleY);
	}
	else if ( m_pInputManager->IsKeyPressed('G'))
	{
		int angleY = int(pCamera->GetOrientationYRad() - 1.f);
		pCamera->SetOrientationY(angleY);
	}


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
		m_pAnimated->SetAnim("Alien_Anim.DAE");
		m_pAnimated->SetAnimFPS(50.f);
		break;
	case RUN :
		break;
	case ATTACK : 
		m_pAnimated->StartAnim();
		m_pAnimated->SetAnim("AnimFrappeDroite.DAE");
		m_pAnimated->SetAnimFPS(20.f);
		break;
	case BOREDOM :
		m_pAnimated->StartAnim();
		m_pAnimated->SetAnim("animBoredom.DAE");
		m_pAnimated->SetAnimFPS(20.f);
		break;
	case BONUS : 
		m_pAnimated->StartAnim();
		m_pAnimated->SetAnim("animBonus.DAE");
		m_pAnimated->SetAnimFPS(20.f);
		break;
	case FLIGHT : 
		break;
	case STATIC :
		m_pAnimated->StopAnim();
		break;
	}


}

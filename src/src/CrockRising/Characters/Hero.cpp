#include "Hero.h"
#include <iostream>
#include "XInput.h"

#include	<Resources/Material.h>
#include	<../CrockRising/Characters/Alien.h>
#include	<../CrockRising/Characters/Alien.h>
#include	<Game/Game.h>
#include	<Core/Macros.h>
#include	<Sound/SoundSystem.h>

#define		MAX_LIFE				100
#define		LIFE_BONUS				10
#define		LEVEL_mainmenu			0x56b55f63

HUDLife* Hero::m_pLifeBar = NULL;

/***********************************************************
* Constructeur
**********************************************************/
Hero::Hero(Vector3f pos)
	:Perso(pos)
{		
	m_pInputManager = InputManager::GetInstance();
	m_pCamera = NULL;
	m_pLifeBar = NULL;
	m_idBone = 21;
}

/**********************************************************
* Initialisation des données membres
**********************************************************/
void Hero::Init()
{
	m_pAnimated = new SceneObjectAnimated("Mesh_Robot.DAE","Anim_Robot_Run.DAE", m_Position); //y = -75 pr le canyon
	m_pAnimated->Init();
	m_pAnimated->GetMaterial()->SetTexture("robot.png", Texture::DIFFUSE);
	m_pAnimated->GetMaterial()->SetTexture("robot_normal.dds", Texture::NORMALMAP);
	m_pAnimated->SetShader("default_skinnormalmap.fx");
 	m_pAnimated->SetRotation(0.f, 180.f,0.f);
	m_pAnimated->SetControledCharacter(1.f,1.f,this);
	m_pAnimated->GetMaterial()->m_Glossiness=10.f;

	InitAnim();
	InitSound();

	m_pArme = new SceneObject( "batte_M.dae", Vector3f( 5, 18, 0)); //en fn de la pos de m_pAnimated
	m_pArme->Init();
	m_pArme->SetRotation(0.f, 90.f, 90.f);
	m_pArme->SetObjectPhysical( "batte_P.dae" ); //pas le bon group!!

	m_pLifeBar = new HUDLife;
	m_pLifeBar->Init();
	m_pLifeBar->SetMaxLife(MAX_LIFE);
	m_pLifeBar->SetLife(50);

	InitWeapon();
}

/***********************************************************
* Destructeur
**********************************************************/
Hero::~Hero()
{	
	release(m_pLifeBar);
}


/*****************************************************************
* Suivant la touche appuyée, change l'état actuel du Héros ou bien 
* gère son déplacement. Cette méthode sert également à la mise à jour 
* de l'orientation de la caméra
******************************************************************/
ResourceResult Hero::control()
{
	changeState(STATIC);

	bool inverseAxeY = true;

//emp manette
	bool manettePresente = true;

	XINPUT_STATE gamepadState;
	DWORD result = XInputGetState( 0, &gamepadState );
	if( result==ERROR_DEVICE_NOT_CONNECTED )
		manettePresente = false;
	

	const float sensibiliteJoystickGauche = 15.0f;
	const float sensibiliteJoystickDroit = 1.f;
	const float coefMulJoystickDroit = 1.0f;

	float X = 0.f, Y = 0.f, 
		  rX = 0.f, rY = 0.f;

	if( manettePresente )
	{
		int x = 0, y = 0,
			rx = 0, ry = 0;

		//Joystick gauche
		if( abs(gamepadState.Gamepad.sThumbLX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE )
		{
			if( gamepadState.Gamepad.sThumbLX > 0 )
				x = gamepadState.Gamepad.sThumbLX - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
			else
				if( gamepadState.Gamepad.sThumbLX < 0 )
					x = gamepadState.Gamepad.sThumbLX + XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

			X = (float)x / (sensibiliteJoystickGauche*(32768-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE));
		}

		if( abs(gamepadState.Gamepad.sThumbLY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE )
		{
			if( gamepadState.Gamepad.sThumbLY > 0 )
				y = gamepadState.Gamepad.sThumbLY - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
			else
				if( gamepadState.Gamepad.sThumbLY < 0 )
					y = gamepadState.Gamepad.sThumbLY + XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

			Y = (float)y / (sensibiliteJoystickGauche*(32768-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE));
		}

		//Joystick droit
		if( abs(gamepadState.Gamepad.sThumbRX) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE )
		{
			if( gamepadState.Gamepad.sThumbRX > 0 )
				rx = gamepadState.Gamepad.sThumbRX - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
			else
				if( gamepadState.Gamepad.sThumbRX < 0 )
					rx = gamepadState.Gamepad.sThumbRX + XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

			rX = (float)rx / (sensibiliteJoystickDroit*(32768-XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE));
		}

		if( abs(gamepadState.Gamepad.sThumbRY) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE )
		{
			if( gamepadState.Gamepad.sThumbRY > 0 )
				ry = gamepadState.Gamepad.sThumbRY - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
			else
				if( gamepadState.Gamepad.sThumbRY < 0 )
					ry = gamepadState.Gamepad.sThumbRY + XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

			rY = (float)ry / (sensibiliteJoystickDroit*(32768-XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE));
		}

		//if(gamepadState.Gamepad.wButtons== XINPUT_GAMEPAD_Y)
		//...
		if( gamepadState.Gamepad.wButtons== XINPUT_GAMEPAD_A )
		{
			changeState(ATTACK);
			return RES_SUCCEED;
		} 

	}

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

	//Joystick droit = mouvement camera
	if( rX != 0.f )
	{
		float offsetCursor = rX * coefMulJoystickDroit;
		float diff = m_pCamera->GetOrientationYRad();
		m_pCamera->SetOrientationY( -offsetCursor );
		diff = m_pCamera->GetOrientationYRad() - diff;
		
		D3DXMATRIX rot;
		D3DXMatrixRotationZ( &rot, diff );
		m_pAnimated->ApplyTransform( &rot );
		m_pAnimated->m_vAngleY -= D3DXToDegree( diff );

		static NxActor* a = m_pArme->getActor() ;
		if( a )
			a->clearBodyFlag( NX_BF_FROZEN_ROT_Y );

		m_pArme->SetRotation( 0, -D3DXToDegree( diff ), 0 );
		
		if( a )
			a->raiseBodyFlag( NX_BF_FROZEN_ROT_Y );
	}
	if( rY != 0.f )
	{
		float offsetCursor = rY * (coefMulJoystickDroit/2);
		if( inverseAxeY ) offsetCursor = -offsetCursor;
		m_pCamera->SetOrientationX( offsetCursor );
	}
	
	//souris
	if( point.x != 0 ) 
	{
		int offsetCursor = (int)point.x%sensibiliteSouris; 
		float diff = m_pCamera->GetOrientationYRad();
		m_pCamera->SetOrientationY( -(float)offsetCursor );
		diff = m_pCamera->GetOrientationYRad() - diff;

		D3DXMATRIX rot;
		D3DXMatrixRotationZ( &rot, diff );
		m_pAnimated->ApplyTransform( &rot );
		//m_pAnimated->SetRotation( 0.f, diff, 0.f );
		m_pAnimated->m_vAngleY -= D3DXToDegree( diff );

		NxActor* a = m_pArme->getActor() ;
		if( a )
			a->clearBodyFlag( NX_BF_FROZEN_ROT_Y );

		m_pArme->SetRotation( 0, -D3DXToDegree( diff ), 0 );
		
		if( a )
			a->raiseBodyFlag( NX_BF_FROZEN_ROT_Y );
	}


	if( point.y != 0 ) 
	{
		int offsetCursor = (int)point.y%sensibiliteSouris; 
		if( inverseAxeY ) offsetCursor = -offsetCursor;
		m_pCamera->SetOrientationX( (float)offsetCursor );
	}


	//Gestion du déplacement du Héros
	float timeF = System::GetInstance()->GetTime()->GetDeltaTimeMs();
	float vitesse = 0.09f; //[0.01; 0.4]
	float sensibilityTranslation = vitesse * timeF;

	m_Translate = Vector3f (0.f, 0.f, 0.f);

	if ( m_pInputManager->IsKeyPressed('Z') || Y != 0.f )
	{
		float xStep, zStep;
		if( Y == 0.f )
		{
			xStep = -std::sin(m_pCamera->GetOrientationYRad())*sensibilityTranslation;
			zStep =  std::cos(m_pCamera->GetOrientationYRad())*sensibilityTranslation;
		}
		else
		{
			xStep = -std::sin( m_pCamera->GetOrientationYRad() )*Y*timeF;
			zStep =  std::cos( m_pCamera->GetOrientationYRad() )*Y*timeF;
		}

		m_Translate += Vector3f(xStep,0.f,zStep);
		changeState(RUN);
	}
	if ( m_pInputManager->IsKeyPressed('Q')  || X != 0.f )
	{
		float xStep, zStep;
		if( X == 0.f )
		{
			xStep = -std::cos( m_pCamera->GetOrientationYRad() )*sensibilityTranslation;
			zStep = -std::sin( m_pCamera->GetOrientationYRad() )*sensibilityTranslation;
		}
		else
		{
			xStep = std::cos( m_pCamera->GetOrientationYRad() )*X*timeF;
			zStep = std::sin( m_pCamera->GetOrientationYRad() )*X*timeF;
		}
		m_Translate += Vector3f(xStep,0.f,zStep);
		changeState(RUN);
	}
	if ( m_pInputManager->IsKeyPressed('S'))
	{
		float xStep, zStep;
		xStep = sin( m_pCamera->GetOrientationYRad() )*sensibilityTranslation;
		zStep = -cos( m_pCamera->GetOrientationYRad() )*sensibilityTranslation;
		m_Translate += Vector3f(xStep,0.f,zStep);
		changeState(RUN);
	}
	if ( m_pInputManager->IsKeyPressed('D'))
	{
		float xStep, zStep;
		xStep = cos( m_pCamera->GetOrientationYRad() )*sensibilityTranslation;
		zStep = sin( m_pCamera->GetOrientationYRad() )*sensibilityTranslation;
		m_Translate += Vector3f(xStep,0.f,zStep);
		changeState(RUN);
	}
	

	return RES_SUCCEED;
}


/*********************************************************************
* Mise à jour du Héros
* Une fois les contrôles éventuels vérifiés plus les changements d'état,
* la position du Héros est modifiée
*********************************************************************/
void Hero::update()
{
	if( m_pCamera ) control();

	if(m_currentState == RUN)
		m_pAnimated->SetTranslation(m_Translate.x, m_Translate.y, m_Translate.z);

	m_Position.x += m_Translate.x;
	m_Position.y += m_Translate.y;
	m_Position.z += m_Translate.z;

	SoundSystem::GetInstance()->SetListenerPosition( m_Position );

	Perso::update();	

	if( m_pCamera )
	{
		m_pCamera->SetTarget(m_pAnimated->GetPosition());
		m_pCamera->Update();
	}
	
	if(m_currentState == DIE && m_pAnimated->IsAtEnd())
		DestroyPerso();
}

void Hero::Hit()
{
	if(m_currentState != DIE)
	{
		changeState(HIT);
		decLife( 5 );
		m_pLifeBar->SetLife( getLife() );
	}
}

void Hero::Die()
{
	changeState(DIE);	
}

void Hero::DestroyPerso()
{
	m_pInputManager->HoldMouseAtCenter(false);
	Game::GetInstance()->ChangeLevel( LEVEL_mainmenu );
}

void Hero::decLife( const int decreaseLife )
{
	Perso::decLife( decreaseLife );
	if( m_pLifeBar ) m_pLifeBar->SetLife( getLife() );
}


void Hero::InitSound()
{
// 	AddSound(Perso::RUN,	"Hit1.ogg");
// 	AddSound(Perso::ATTACK, "Attack1.ogg");
// 	AddSound(Perso::ATTACK, "Attack2.ogg");
// 	AddSound(Perso::ATTACK, "Attack3.ogg");
// 	AddSound(Perso::HIT,	"Hit1.ogg");
// 	AddSound(Perso::HIT,	"Hit2.ogg");
// 	AddSound(Perso::HIT,	"Hit3.ogg");
// 	AddSound(Perso::DIE,	"Die1.ogg");
// 	AddSound(Perso::DIE,	"Die2.ogg");
// 	AddSound(Perso::DIE,	"Die3.ogg");
}

void Hero::InitAnim()
{
	AddAnim(Perso::RUN,		"Anim_Robot_Run.DAE"	, 50.f, true);
	AddAnim(Perso::ATTACK,	"Anim_Robot_Attack.DAE"	, 50.f, false);
	AddAnim(Perso::HIT,		"Anim_Robot_Hit.DAE"	, 30.f, false);
	AddAnim(Perso::DIE,		"Anim_Robot_Die.DAE"	, 50.f, false);
	AddAnim(Perso::STATIC,	"Anim_Robot_Wait.DAE"	, 30.f, true);
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
		capsule->SetObjectUnPhysical();
		capsule->SetVisible(false);
	}
}
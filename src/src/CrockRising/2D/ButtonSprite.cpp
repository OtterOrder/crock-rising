#include	"ButtonSprite.h"

#include	<assert.h>
#include	<Core/Inputs/InputManager.h>
#include	<Game/Game.h>
#include	<Sound/SoundObject.h>

//******************************************************************

#define		BS_DEFAULT_WAIT_COLOR	(Color4f( 0.5f, 0.5f, 0.5f, 1.f ))
#define		BS_DEFAULT_HOVER_COLOR	(Color4f( 1.f, 1.f, 1.f, 1.f ))

//******************************************************************

//**********************************************************
// Constructeur.
//**********************************************************
ButtonSprite::ButtonSprite( const std::string &spriteName )
: Sprite( spriteName )
{
	m_IsActive = true;
	m_WaitColor = BS_DEFAULT_WAIT_COLOR;
	m_HoverColor = BS_DEFAULT_HOVER_COLOR;
	m_pSound = NULL;

	SetColor( m_WaitColor );
	SetState( WAIT );
}

//**********************************************************
// Update.
//**********************************************************
void ButtonSprite::Update()
{
	InputManager *inputMgr = InputManager::GetInstance();
	Vector2f mousePos = inputMgr->GetMousePosition();

	if( m_IsActive										&&
		IsCollide( (s32)mousePos.x, (s32)mousePos.y )	)
	{
		SetState( HOVER );
		if( inputMgr->IsMouseReleased( MOUSE_LEFT ) )
		{
			OnClic();
		}
		return;
	}
	SetState( WAIT );
}

//**********************************************************
// Active/désactive le bouton.
// @param[in]	isActive : Vrai = bouton actif
//**********************************************************
void ButtonSprite::Activate( bool isActive )
{
	m_IsActive = isActive;
}

//**********************************************************
// Change la couleur d'attente.
// @param[in]	color : Couleur du texte
//**********************************************************
void ButtonSprite::SetWaitColor( const Color4f &color )
{
	m_WaitColor = color;
	SetColor( m_WaitColor );
}

//**********************************************************
// Change la couleur de survole.
// @param[in]	color : Couleur du texte
//**********************************************************
void ButtonSprite::SetHoverColor( const Color4f &color )
{
	m_HoverColor = color;
}

//**********************************************************
// Indique le son à jouer lorsque le lien est survolé.
// Attention, le son n'est ni créé ni détruit par le
// bouton, il faut le gérer correctement en dehors.
// Donnez NULL pour ne jouer aucun son..
// @param[in]	pSound : Pointeur sur le son à jouer
//**********************************************************
void ButtonSprite::SetSound( SoundObject *pSound )
{
	m_pSound = pSound;
}

//**********************************************************
// Change l'état du bouton.
//**********************************************************
void ButtonSprite::SetState( State state )
{
	if( state == m_State )
		return;

	switch( m_State = state )
	{
		case WAIT:
			SetColor( m_WaitColor );
			break;

		case HOVER:
			if( m_pSound )
			{
				m_pSound->Play();
			}
			SetColor( m_HoverColor );
			break;

		default:
			// Pas d'autre cas possible..
			assert( false );
	}
}

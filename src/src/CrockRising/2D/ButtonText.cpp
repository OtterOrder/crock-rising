#include	"ButtonText.h"

#include	<assert.h>
#include	<Core/Inputs/InputManager.h>
#include	<Game/Game.h>
#include	<Sound/SoundObject.h>

//******************************************************************

#define		LEVEL_mainmenu			0x56b55f63

#define		BT_DEFAULT_FONT			"Arial"
#define		BT_DEFAULT_SIZE			17
#define		BT_DEFAULT_WAIT_COLOR	(Color4f( 0.f, 0.f, 0.0f, 0.5f ))
#define		BT_DEFAULT_HOVER_COLOR	(Color4f( 0.f, 0.f, 0.f, 1.f ))

//******************************************************************

//**********************************************************
// Initialisation commune � tous les constructeurs.
//**********************************************************
void ButtonText::CommonInit()
{
	m_IsActive = true;
	m_WaitColor = BT_DEFAULT_WAIT_COLOR;
	m_HoverColor = BT_DEFAULT_HOVER_COLOR;
	m_pSound = NULL;

	SetColor( m_WaitColor );
	SetState( WAIT );
}

//**********************************************************
// Constructeur.
//**********************************************************
ButtonText::ButtonText( const std::string &text )
: Text( text, BT_DEFAULT_FONT, BT_DEFAULT_SIZE, true, false )
{
	CommonInit();
}

//**********************************************************
// Constructeur avec param�tres.
//**********************************************************
ButtonText::ButtonText( const std::string &text, const std::string &fontName, u32 size, bool bold, bool italic )
: Text( text, fontName, size, bold, italic )
{
	CommonInit();
}

//**********************************************************
// Update.
//**********************************************************
void ButtonText::Update()
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
// Active/d�sactive le bouton.
// @param[in]	isActive : Vrai = bouton actif
//**********************************************************
void ButtonText::Activate( bool isActive )
{
	m_IsActive = isActive;
}

//**********************************************************
// Change la couleur d'attente.
// @param[in]	color : Couleur du texte
//**********************************************************
void ButtonText::SetWaitColor( const Color4f &color )
{
	m_WaitColor = color;
	SetColor( m_WaitColor );
}

//**********************************************************
// Change la couleur de survole.
// @param[in]	color : Couleur du texte
//**********************************************************
void ButtonText::SetHoverColor( const Color4f &color )
{
	m_HoverColor = color;
}

//**********************************************************
// Indique le son � jouer lorsque le lien est survol�.
// Attention, le son n'est ni cr�� ni d�truit par le
// bouton, il faut le g�rer correctement en dehors.
// Donnez NULL pour ne jouer aucun son..
// @param[in]	pSound : Pointeur sur le son � jouer
//**********************************************************
void ButtonText::SetSound( SoundObject *pSound )
{
	m_pSound = pSound;
}

//**********************************************************
// Change l'�tat du bouton.
//**********************************************************
void ButtonText::SetState( State state )
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

//**********************************************************
// Constructeur de BackButton.
//**********************************************************
BackButton::BackButton()
: ButtonText( "RETOUR" )
{
	SetBubbleWidth( 60 );
}

//**********************************************************
// Action au clic sur un bouton.
//**********************************************************
void BackButton::OnClic()
{
	// Retour au menu principal..
	Game::GetInstance()->ChangeLevel( LEVEL_mainmenu );
}
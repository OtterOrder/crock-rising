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
// Constructeur.
//**********************************************************
ButtonText::ButtonText( const std::string &text )
: Text( text, BT_DEFAULT_FONT, BT_DEFAULT_SIZE, true, false )
{
	m_WaitColor = BT_DEFAULT_WAIT_COLOR;
	m_HoverColor = BT_DEFAULT_HOVER_COLOR;
	m_pSound = NULL;
	SetState( WAIT );
}

//**********************************************************
// Constructeur avec paramètres.
//**********************************************************
ButtonText::ButtonText( const std::string &text, const std::string &fontName, u32 size, bool bold, bool italic )
: Text( text, fontName, size, bold, italic )
{
	m_WaitColor = BT_DEFAULT_WAIT_COLOR;
	m_HoverColor = BT_DEFAULT_HOVER_COLOR;
	m_pSound = NULL;
	SetState( WAIT );
}

//**********************************************************
// Update.
//**********************************************************
void ButtonText::Update()
{
	InputManager *inputMgr = InputManager::GetInstance();
	Vector2f mousePos = inputMgr->GetMousePosition();

	if( IsCollide( (s32)mousePos.x, (s32)mousePos.y ) )
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
// Change la couleur d'attente.
// @param[in]	color : Couleur du texte
//**********************************************************
void ButtonText::SetWaitColor( const Color4f &color )
{
	m_WaitColor = color;
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
// Indique le son à jouer lorsque le lien est survolé.
// Attention, le son n'est ni créé ni détruit par le
// bouton, il faut le gérer correctement en dehors.
// Donnez NULL pour ne jouer aucun son..
// @param[in]	pSound : Pointeur sur le son à jouer
//**********************************************************
void ButtonText::SetSound( SoundObject *pSound )
{
	m_pSound = pSound;
}

//**********************************************************
// Change l'état du bouton.
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

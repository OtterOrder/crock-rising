#include	"EscMenu.h"

#include	<Core/System.h>
#include	<Core/Time.h>
#include	<Core/Inputs/InputManager.h>
#include	<Game/Game.h>
#include	<Renderer/Renderer.h>

//******************************************************************

#define		LEVEL_mainmenu				0x56b55f63

#define		ESC_BG_COLOR				Color4f( 0.f, 0.f, 0.f, 0.91f )

#define		ESC_BUTTON_FONT				"Arial"
#define		ESC_BUTTON_SIZE				25
#define		ESC_BUTTON_WIDTH			250
#define		ESC_BUTTON_MARGIN			30
#define		ESC_BUTTON_WAIT_COLOR		Color4f( 0.6f, 0.6f, 0.6f, 1.f )
#define		ESC_BUTTON_HOVER_COLOR		Color4f( 1.f, 1.f, 1.f, 1.f )

#define		ESC_APPEARANCE_TIME			0.15f	// Temps d'apparition/disparition du menu (s)

//******************************************************************

static const char *g_ButtonsText[EscMenu::NB_LINKS] =
{
	"REPRENDRE",
	"MENU PRINCIPAL",
	"QUITTER"
};

//******************************************************************

//**********************************************************
// Constructeur.
// @param[in]	isMouseFree : Si la souris doit être libre
//				en dehors du menu. Typiquement false dans
//				un niveau de jeu..
//**********************************************************
EscMenu::EscMenu( bool isMouseFree )
: Quad( 0, 0, ESC_BG_COLOR )
{
	m_State			= CLOSED;
	m_IsMouseFree	= isMouseFree;
	m_AppTimer		= 0.f;

	memset( m_Menu, NULL, NB_LINKS*sizeof(EscButton*) );
}

//**********************************************************
// Destructeur.
//**********************************************************
EscMenu::~EscMenu()
{
	for( int link = 0; link < NB_LINKS; link++ )
		if( m_Menu[link] )
			delete m_Menu[link];
}

//**********************************************************
// Init de l'objet.
//**********************************************************
void EscMenu::Init()
{
	Quad::Init();

	// Initialisation du menu..
	for( int link = 0; link < NB_LINKS; link++ )
	{
		m_Menu[link] = new EscButton( (Link)link, this );
		m_Menu[link]->Init();
		
		m_Menu[link]->SetText( g_ButtonsText[link] );
		m_Menu[link]->SetFormat( DT_TOP | DT_CENTER );
		m_Menu[link]->SetBubbleSize( ESC_BUTTON_WIDTH, ESC_BUTTON_SIZE );
		m_Menu[link]->SetWaitColor( ESC_BUTTON_WAIT_COLOR );
		m_Menu[link]->SetHoverColor( ESC_BUTTON_HOVER_COLOR );
		m_Menu[link]->SetHotPoint( ESC_BUTTON_WIDTH/2, 0 );

		m_Menu[link]->Activate( false );
		m_Menu[link]->SetAsPausable( false );
		m_Menu[link]->SetPriority( 10 );
	}

	SetAsPausable( false );
	SetPriority( 11 );
	SetAlpha( 0.f );
}

//**********************************************************
// Update.
//**********************************************************
void EscMenu::Update()
{
	switch( m_State )
	{
		case CLOSED:
			// On ouvre le menu quand on appuie sur ESCAPE..
			if( InputManager::GetInstance()->IsKeyReleased( VK_ESCAPE ) )
			{
				Open();
			}
			return;

		case OPENING:
		{
			m_AppTimer -= System::GetInstance()->GetTime()->GetDeltaTime();

			// Le menu apparait progressivement..
			SetAlpha( (ESC_APPEARANCE_TIME-m_AppTimer)/ESC_APPEARANCE_TIME );

			if( m_AppTimer <= 0.f )
			{
				m_State = OPENED;
			}
			return;
		}

		case OPENED:
			// On ferme le menu quand on appuie sur ESCAPE..
			if( InputManager::GetInstance()->IsKeyReleased( VK_ESCAPE ) )
			{
				Close();
			}
			return;

		case CLOSING:
		{
			m_AppTimer -= System::GetInstance()->GetTime()->GetDeltaTime();

			// Le menu disparait progressivement..
			SetAlpha( m_AppTimer/ESC_APPEARANCE_TIME );

			if( m_AppTimer <= 0.f )
			{
				// On relance le jeu..
				Game::GetInstance()->Resume();
				m_State = CLOSED;
			}
			return;
		}
	}
}

//**********************************************************
// Ouvre le menu.
//**********************************************************
void EscMenu::Open()
{
	Renderer *pRenderer = Renderer::GetInstance();
	u32 wWidth	= pRenderer->GetWindowWidth();
	u32 wHeight	= pRenderer->GetWindowHeight();

	// On pause le jeu..
	Game::GetInstance()->Pause();

	// On libère la souris
	InputManager::GetInstance()->HoldMouseAtCenter( false );
	InputManager::GetInstance()->ShowOSCursor( true );

	// Le quad recouvre toute la fenêtre
	SetSize( wWidth, wHeight );
	SetPosition( 0, 0 );

	// Positionnement du menu
	int posY = wHeight/2 - (NB_LINKS*ESC_BUTTON_MARGIN)/2;
	for( int link = 0; link < NB_LINKS; link++ )
	{
		m_Menu[link]->SetPosition( wWidth/2, posY + link*ESC_BUTTON_MARGIN );
		m_Menu[link]->Activate( true );
	}

	m_AppTimer	= ESC_APPEARANCE_TIME;
	m_State		= OPENING;
}

//**********************************************************
// Ferme le menu.
//**********************************************************
void EscMenu::Close()
{
	// Désactivation des liens..
	for( int link = 0; link < NB_LINKS; link++ )
		m_Menu[link]->Activate( false );

	if( !m_IsMouseFree )
	{
		// La souris ne doit pas être libre
		InputManager::GetInstance()->HoldMouseAtCenter( true );
		InputManager::GetInstance()->ShowOSCursor( false );
	}
	
	m_AppTimer	= ESC_APPEARANCE_TIME;
	m_State		= CLOSING;
}

//**********************************************************
// Change l'alpha.
//**********************************************************
void EscMenu::SetAlpha( float alpha )
{
	// Alpha du fond
	float bgAlpha = alpha*ESC_BG_COLOR.a;
	Quad::SetAlpha( MATH_Clamp( bgAlpha, 0.f, ESC_BG_COLOR.a ) );

	// Alpha des boutons
	float buttonAlpha = alpha*ESC_BUTTON_WAIT_COLOR.a;
	buttonAlpha = MATH_Clamp( buttonAlpha, 0.f, ESC_BUTTON_WAIT_COLOR.a );

	for( int link = 0; link < NB_LINKS; link++ )
		m_Menu[link]->SetAlpha( buttonAlpha );
}

//**********************************************************
// Constructeur de EscButton.
// @param[in]	_linkID	: ID du lien
// @param[in]	_pMenu	: Pointeur sur le menu englobant
//**********************************************************
EscMenu::EscButton::EscButton( Link _linkID, EscMenu *_pMenu )
: ButtonText( "", ESC_BUTTON_FONT, ESC_BUTTON_SIZE, true, false )
{
	linkID	= _linkID;
	pMenu	= _pMenu;
}

//**********************************************************
// Action au clic sur un bouton.
//**********************************************************
void EscMenu::EscButton::OnClic()
{
	switch( linkID )
	{
		case BACK:
			if( pMenu )
			{
				// Ferme le EscMenu
				pMenu->Close();
			}
			return;

		case MAINMENU:
			// Menu principal (sans oublier de désactiver la pause)..
			Game::GetInstance()->Resume();
			Game::GetInstance()->ChangeLevel( LEVEL_mainmenu );
			return;

		case QUITGAME:
			// Fermeture de l'appli..
			System::GetInstance()->Shutdown();
			return;
	}
}

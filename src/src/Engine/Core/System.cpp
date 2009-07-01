#include	"System.h"

#include	"Core/Inputs/InputManager.h"
#include	"Renderer/Renderer.h"
#include	"PostProcesses/PostRenderer.h"
#include	"Sound/SoundSystem.h"
#include	"Resources/ResourceManager.h"
#include	"Game/Game.h"
#include	"Physics/Physicalizer.h"
#include	"Core/Time.h"

#define		NB_FPS			120
#define		FRAME_DELAY		(1.f/NB_FPS)

//******************************************************************

/***********************************************************
 * Constructeur.
 **********************************************************/
System::System( void )
{
	m_Time				= NULL;
	m_FullScreen		= false;
	m_FullScreenWidth	= 0;
	m_FullScreenHeight	= 0;
}

/***********************************************************
 * Destructeur.
 **********************************************************/
System::~System( void )
{
	if( m_Time )
		delete m_Time;
}

/***********************************************************
 * Initialise le System.
 **********************************************************/
void System::Init( void )
{
	// Gestion du temps
	m_Time = new Time;
	
	// Cr�ation du device, initialisation D3D, cr�ation de la fen�tre
	if( FAILED( InitRenderer() ) )
	{
		MessageBox( NULL, "Initialisation du Renderer impossible", "Erreur System", MB_ICONERROR );
	}
}

/***********************************************************
 * Termine l'appli. Sort de la main loop proprement.
 **********************************************************/
void System::Shutdown( void )
{
	// Evenement de sortie de la boucle..
	PostQuitMessage( 0 );
}

/***********************************************************
 * Set les propri�t�s de plein �cran. Cette m�thode n'est
 * valable qu'avant l'appel de System::Init (c'est du bricolage
 * mais bon on fait ce qu'on peut).
 **********************************************************/
void System::SetFullScreen( bool fullScreen, int width, int height )
{
	m_FullScreen		= fullScreen;
	m_FullScreenWidth	= width;
	m_FullScreenHeight	= height;
}

/***********************************************************
 * Boucle principale de l'application.
 **********************************************************/
int System::MainLoop( void )
{
	ResourceManager	*resourceManager;
	InputManager	*inputManager;
	Renderer		*renderer;
	PostRenderer	*postRenderer;
	SoundSystem		*soundSystem;
	Game			*game;
	Physicalizer	*physX;

	bool			bGotMsg;
	MSG				msg;

	// On initialise les modules..
	renderer		= Renderer::GetInstance();
	postRenderer	= PostRenderer::GetInstance();
	resourceManager	= ResourceManager::GetInstance();
	inputManager	= InputManager::GetInstance();
	soundSystem		= SoundSystem::GetInstance();
	physX			= Physicalizer::GetInstance();
	game			= Game::GetInstance();

	// Tous les modules sont initialis�s, on lance le jeu..
	game->Start();
	
	msg.message = WM_NULL;
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );
	
	// Boucle principale..
	while( msg.message != WM_QUIT )
    {
		// Use PeekMessage() if the app is active, so we can use idle time to
        // render the scene. Else, use GetMessage() to avoid eating CPU time.
		
		bGotMsg = ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 );
		
		if( bGotMsg )
		{
			// Translate and dispatch the message
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			// Update de la scene
			game->Update();
			
			// Update de physX
			physX->RunPhysics( m_Time->GetDeltaTime() );

			if( m_Time->GetDeltaTimeF() >= FRAME_DELAY )
			{				
				// Rendu de de la scene
				renderer->Run();
				
				// Post effects
				postRenderer->RenderPostEffects();
				
				m_Time->EndF();	// Fin de la frame
			}

			// Update du son
			//soundSystem->Update();
			
			// Update des inputs
			inputManager->Update();

			m_Time->EndE();	// Fin du tour moteur
		}
	}

	// Destruction des singletons
	game->Destroy(); // Game � d�truire en premier !
	physX->Destroy();
	soundSystem->Destroy();
	inputManager->Destroy();
	
	renderer->Close();
	renderer->Destroy();

	postRenderer->Destroy();

	resourceManager->Destroy(); // Normalement il est vide

	return (int)msg.wParam;
}

/***********************************************************
 * Donne le gestionnaire de temps.
 * @return	Pointeur sur le gestionnaire de temps
 **********************************************************/
Time* const System::GetTime() const
{
	return m_Time;
}

/***********************************************************
 * Fonction de rappel des �v�nements.
 * Voir MSDN > WindowProc pour plus d'infos.
 * @param[in]	hWnd	: handle de la fen�tre
 * @param[in]	uMsg	: message
 * @param[in]	wParam	: infos suppl�mentaires sur le message
 * @param[in]	lParam	: infos suppl�mentaires sur le message
 * @return	un code r�sultat
 **********************************************************/
LRESULT CALLBACK System::EventsCallback( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult;
	
	// On appelle les callbacks d'�venements de tous les modules
	lResult = InputManager::GetInstance()->EventsCallback( hWnd, uMsg, wParam, lParam );
	lResult = Renderer::GetInstance()->EventsCallback( hWnd, uMsg, wParam, lParam );

	//..

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

/***********************************************************
 * Initialise le Renderer. Normalement cette fonction
 * doit �tre la PREMIERE du programme � appeler le Renderer.
 **********************************************************/
HRESULT System::InitRenderer( void )
{
	WNDCLASS wndClass;

	// Register the windows class
	wndClass.style			= CS_DBLCLKS;
	wndClass.lpfnWndProc	= System::EventsCallback;
	wndClass.cbClsExtra		= 0;
	wndClass.cbWndExtra		= 0;
	wndClass.hInstance		= m_Instance;
	wndClass.hIcon			= m_Icon;
	wndClass.hCursor		= LoadCursor( NULL, IDC_ARROW );
	wndClass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName	= NULL;
	wndClass.lpszClassName	= "Direct3DWindowClass";

	if( m_FullScreen )
	{
		// Il faut initialiser la r�solution de plein �cran
		// avant la cr�ation de la fen�tre, et pis c'est tout !
		Renderer::GetInstance()->SetFullScreenResolution(
			m_FullScreenWidth,
			m_FullScreenHeight
			);
	}

	// Cr�ation de tout le bordel du Renderer..
	return Renderer::GetInstance()->Create( m_Instance, wndClass );
}

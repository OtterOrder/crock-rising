#include	"System.h"

#include	"Core/Inputs/InputManager.h"
#include	"Renderer/Renderer.h"
#include	"Game/Game.h"

//******************************************************************

bool System::s_IsInitialized = false;

/***********************************************************
 * Constructeur.
 **********************************************************/
System::System( void )
{
	s_IsInitialized	= false;
	
	m_Game			= NULL;
	m_Renderer		= NULL;
	m_InputManager	= NULL;
}

/***********************************************************
 * Destructeur.
 **********************************************************/
System::~System( void )
{
	if( s_IsInitialized )
	{
		// On détruit tous les singletons
		m_Game->Destroy();
		m_Renderer->Destroy();
		m_InputManager->Destroy();
	}
}

/***********************************************************
 * Initialise l'appli.
 **********************************************************/
void System::Initialize	( void )
{
	// On récupère les instances de tous les singletons pour un
	// traitement plus rapide dans la boucle principale. Par ailleurs,
	// ça permet d'être sûr que tous les singletons ont une instance
	// créée avant le lancement du jeu.
	
	m_Game			= Game::GetInstance();
	m_Renderer		= Renderer::GetInstance();
	m_InputManager	= InputManager::GetInstance();

	//TODO

	s_IsInitialized = true;
}

/***********************************************************
 * Boucle principale de l'application.
 **********************************************************/
int System::MainLoop( void )
{
	if( !s_IsInitialized )
	{
		printf( "System non intialisé." );
		return -1;
	}
	
	bool	bGotMsg;
	MSG		msg;
	
	msg.message = WM_NULL;
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );
	
	Camera cam( Vector3f(0.0f, 50.0f, -170.0f), Vector3f(0.f, 1.f, 0.f), Vector3f(0.f, 50.f, 0.f));
	
	//Création du device, initialisation D3D, création de la fenêtre
	if(FAILED(InitWindow()))
		return 0;
	
	m_Renderer->SetCamera( &cam );

    while( WM_QUIT != msg.message )
    {
		// Use PeekMessage() if the app is active, so we can use idle time to
        // render the scene. Else, use GetMessage() to avoid eating CPU time.
		
		bGotMsg = ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 );
		
		// Update des inputs
		m_InputManager->Update();
		
		if( bGotMsg )
		{
			// Translate and dispatch the message
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			//Update de la scene
			m_Game->Update();
			
			//Rendu de de la scene
			m_Renderer->Run();
		}
	}
	return (INT)msg.wParam;
}


/***********************************************************
 * Fonction de rappel des événements.
 * Voir MSDN > WindowProc pour plus d'infos.
 * @param[in]	hWnd	: handle de la fenêtre
 * @param[in]	uMsg	: message
 * @param[in]	wParam	: infos supplémentaires sur le message
 * @param[in]	lParam	: infos supplémentaires sur le message
 * @return	un code résultat
 **********************************************************/
LRESULT CALLBACK System::EventsCallback( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( !s_IsInitialized )
	{
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}
	
	// Pour la gestion des erreurs
	LRESULT lResult;
	
	// On appelle les callbacks d'évenements de tous les modules
	lResult = InputManager::GetInstance()->EventsCallback( hWnd, uMsg, wParam, lParam );
	lResult = Renderer::GetInstance()->EventsCallback( hWnd, uMsg, wParam, lParam );

	//..

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

/***********************************************************
 * Initialise la fenêtre.
 **********************************************************/
HRESULT System::InitWindow()
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

	return Renderer::GetInstance()->Create( m_Instance, wndClass );
}

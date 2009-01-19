#include	"System.h"

#include	"Core/Inputs/InputManager.h"
#include	"Renderer/Renderer.h"
#include	"Game/Game.h"


//******************************************************************

/***********************************************************
 * Constructeur.
 **********************************************************/
System::System( void )
{
}

/***********************************************************
 * Destructeur.
 **********************************************************/
System::~System( void )
{
}

/***********************************************************
 * Boucle principale de l'application.
 **********************************************************/
int System::MainLoop( void )
{
	Renderer		*renderer;
	InputManager	*inputManager;
	Game			*game;
	bool			bGotMsg;
	MSG				msg;

	// On initialise les singletons ici pour ne
	// pas avoir à le faire dans la boucle
	renderer		= Renderer::GetInstance();
	inputManager	= InputManager::GetInstance();
	game			= Game::GetInstance();
	
	msg.message = WM_NULL;
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );
	
	// Création du device, initialisation D3D, création de la fenêtre
	if( FAILED( InitWindow() ) )
		return 0;
	
	Camera cam( Vector3f(0.0f, 50.0f, -170.0f), Vector3f(0.f, 50.f, 0.f), Vector3f(0.f, 1.f, 0.f));
	renderer->SetCamera( &cam );

    // Boucle principale..
	while( msg.message != WM_QUIT )
    {
		// Use PeekMessage() if the app is active, so we can use idle time to
        // render the scene. Else, use GetMessage() to avoid eating CPU time.
		
		bGotMsg = ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 );
		
		// Update des inputs
		inputManager->Update();
		
		if( bGotMsg )
		{
			// Translate and dispatch the message
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			//Update de la scene
			game->Update();
			
			//Rendu de de la scene
			renderer->Run();
		}
	}

	// Destruction des singletons
	renderer->Destroy();
	inputManager->Destroy();
	game->Destroy();

	return (int)msg.wParam;
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

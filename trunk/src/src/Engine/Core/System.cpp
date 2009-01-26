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
	// Cr�ation du device, initialisation D3D, cr�ation de la fen�tre
	if( FAILED( InitWindow() ) )
		MessageBox(NULL, "Initialisation de la fenetre impossible", "Erreur System", MB_ICONERROR);
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
	// pas avoir � le faire dans la boucle
	renderer		= Renderer::GetInstance();
	inputManager	= InputManager::GetInstance();
	game			= Game::GetInstance();
	
	msg.message = WM_NULL;
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );
	/*
	// Cr�ation du device, initialisation D3D, cr�ation de la fen�tre
	if( FAILED( InitWindow() ) )
		return 0;*/
	

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
			//Update de la scene
			game->Update();
			
			//Rendu de de la scene
			renderer->Run();

			// Update des inputs
			inputManager->Update();
		}
	}

	// Destruction des singletons
	renderer->Destroy();
	inputManager->Destroy();
	game->Destroy();

	return (int)msg.wParam;
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
 * Initialise la fen�tre.
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

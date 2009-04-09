#include	"System.h"

#include	"Core/Inputs/InputManager.h"
#include	"Renderer/Renderer.h"
#include	"PostProcesses/PostRenderer.h"
#include	"Resources/ResourceManager.h"
#include	"Game/Game.h"
#include	"Physics/Physicalizer.h"
#include	"Time.h"

#define FPS_MAX 80
//******************************************************************

/***********************************************************
 * Constructeur.
 **********************************************************/
System::System( void )
{
	// Création du device, initialisation D3D, création de la fenêtre
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
	ResourceManager	*resourceManager;
	InputManager	*inputManager;
	Renderer		*renderer;
	PostRenderer	*postRenderer;
	Game			*game;
	Physicalizer	*physX;

	bool			bGotMsg;
	MSG				msg;

	// On initialise les singletons ici pour
	// ne pas avoir à le faire dans la boucle
	resourceManager	= ResourceManager::GetInstance();
	inputManager	= InputManager::GetInstance();
	renderer		= Renderer::GetInstance();
	postRenderer	= PostRenderer::GetInstance();
	game			= Game::GetInstance();
	physX			= Physicalizer::GetInstance();
	
	msg.message = WM_NULL;
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );
	
	Time            *time = new Time();
	unsigned int    nbFps = 1000 / FPS_MAX;
	
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

			//Post effects
			postRenderer->RenderPostEffects();

			// Update des inputs
			inputManager->Update();

			//Update de physX				
			if (time->getDeltaTimeF() >= nbFps) 
			{
				physX->RunPhysics();
				time->resetDeltaTimeF();    // Reset du temps entre les 2 frames
			}
		}
	}

	// Destruction des singletons
	game->Destroy(); // Game à détruire en premier !
	renderer->Destroy();
	postRenderer->Destroy();
	inputManager->Destroy();
	resourceManager->Destroy();

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

#include "System.h"


//-----------------------------------------------------------------------------
// Name: WndProc()
// Desc: Static msg handler which passes messages to the application class.
//-----------------------------------------------------------------------------


HRESULT System::InitWindow()
{

	// Register the windows class
	WNDCLASS wndClass;
	wndClass.style = CS_DBLCLKS;
	wndClass.lpfnWndProc = System::EventsCallback;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = m_Instance;
	wndClass.hIcon = m_hIcon;
	wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = "Direct3DWindowClass";

	return Renderer::GetInstance()->Create(m_Instance, wndClass);

}


int System::MainLoop()
{

	bool bGotMsg;
    MSG  msg;
    msg.message = WM_NULL;
    PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

	Camera cam( Vector3f(0.0f, 2.0f, -170.0f));
	//Création du device, initialisation D3D, création de la fenêtre
	if(FAILED(InitWindow()))
		return 0;
	
	Renderer::GetInstance()->SetCamera( &cam );

    while( WM_QUIT != msg.message  )
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


			//Rendu de de la scene
			Renderer::GetInstance()->Run();
            
        }


    } ; 

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
	// Pour la gestion des erreurs
	LRESULT lResult;
	
	// Callback de inputs
	lResult = InputManager::GetInstance()->EventsCallback( hWnd, uMsg, wParam, lParam );
	
	// Callback de événements de rendu
	lResult = Renderer::GetInstance()->EventsCallback( hWnd, uMsg, wParam, lParam );

	//TODO

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}


#include "System.h"



int System::MainLoop()
{

	bool bGotMsg;
    MSG  msg;
    msg.message = WM_NULL;
    PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

	//Création du device, initialisation D3D, création de la fenêtre
	Renderer::GetInstance()->Create(m_Instance, NULL);

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


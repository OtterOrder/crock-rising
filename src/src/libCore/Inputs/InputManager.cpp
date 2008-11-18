#include "InputManager.h"

//******************************************************************

/***********************************************************
 * Vérifie si la touche est appuyée.
 * @param[in]	key	: touche à vérifier
 * @return	true si la touche est appuyée, false sinon
 **********************************************************/
bool InputManager::IsKeyTriggered( int key )
{
	//TODO
	
	return false;
}

/***********************************************************
 * Vérifie si la touche est maintenue appuyée.
 * @param[in]	key	: touche à vérifier
 * @return	true si la touche est maintenue appuyée,
 *			false sinon
 **********************************************************/
bool InputManager::IsKeyPressed( int key )
{
	//TODO
	
	return false;
}

/***********************************************************
 * Vérifie si la touche est relachée.
 * @param[in]	key	: touche à vérifier
 * @return	true si la touche est relachée, false sinon
 **********************************************************/
bool InputManager::IsKeyReleased( int key )
{
	//TODO

	return false;
}


/***********************************************************
 * Donne le vecteur de la souris.
 * @return	vecteur de la souris
 **********************************************************/
Vector3f InputManager::GetMouseVector( void ) const
{
	return m_MouseVector;
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
LRESULT CALLBACK InputManager::EventsCallback( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
		// Touche du clavier enfoncée
		case WM_KEYDOWN:
			//TODO
			break;

		// Touche du clavier relachée
		case WM_KEYUP:
			//TODO
			break;

		// ..
		case WM_LBUTTONDOWN:
			//TODO
			break;

		// Mouvement de la souris
		case WM_MOUSEMOVE:
			//m_MouseX = GET_X_LPARAM( lParam );
			//m_MouseY = GET_Y_LPARAM( lParam );
			break;

	}
	return S_OK;
}


/***********************************************************
 * Constructeur.
 **********************************************************/
InputManager::InputManager( void )
{
	m_MouseX	= 0;
	m_MouseY	= 0;
	
	//m_LastMousePosition	= g_NullVector3f;
	//m_MouseVector		= g_NullVector3f;
}

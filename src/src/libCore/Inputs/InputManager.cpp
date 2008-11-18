#include "InputManager.h"

//******************************************************************

/***********************************************************
 * V�rifie si la touche est appuy�e.
 * @param[in]	key	: touche � v�rifier
 * @return	true si la touche est appuy�e, false sinon
 **********************************************************/
bool InputManager::IsKeyTriggered( int key )
{
	//TODO
	
	return false;
}

/***********************************************************
 * V�rifie si la touche est maintenue appuy�e.
 * @param[in]	key	: touche � v�rifier
 * @return	true si la touche est maintenue appuy�e,
 *			false sinon
 **********************************************************/
bool InputManager::IsKeyPressed( int key )
{
	//TODO
	
	return false;
}

/***********************************************************
 * V�rifie si la touche est relach�e.
 * @param[in]	key	: touche � v�rifier
 * @return	true si la touche est relach�e, false sinon
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
 * Fonction de rappel des �v�nements.
 * Voir MSDN > WindowProc pour plus d'infos.
 * @param[in]	hWnd	: handle de la fen�tre
 * @param[in]	uMsg	: message
 * @param[in]	wParam	: infos suppl�mentaires sur le message
 * @param[in]	lParam	: infos suppl�mentaires sur le message
 * @return	un code r�sultat
 **********************************************************/
LRESULT CALLBACK InputManager::EventsCallback( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
		// Touche du clavier enfonc�e
		case WM_KEYDOWN:
			//TODO
			break;

		// Touche du clavier relach�e
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

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
 * Donne la position de la souris.
 * @return	position de la souris
 **********************************************************/
Point2f InputManager::GetMousePosition( void ) const
{
	return m_MousePosition;
}

/***********************************************************
 * Donne le vecteur de la souris (depuis le dernier
 * tour moteur).
 * @return	vecteur de la souris
 **********************************************************/
Vector2f InputManager::GetMouseVector( void ) const
{
	return m_MousePosition - m_MouseOldPosition;
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
	POINTS mousePt;
	
	m_MouseOldPosition = m_MousePosition;

	switch( uMsg )
	{
		case WM_MOUSEMOVE:
			mousePt				= MAKEPOINTS( lParam );
			m_MousePosition.x	= mousePt.x;
			m_MousePosition.y	= mousePt.y;
			//break;
		
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

	}
	return S_OK;
}


/***********************************************************
 * Constructeur.
 **********************************************************/
InputManager::InputManager( void )
{
	m_MouseOldPosition	= Point2f( 0.f, 0.f );
	m_MousePosition		= Point2f( 0.f, 0.f );

	m_IsMouseMoving		= false;

	//TODO
}

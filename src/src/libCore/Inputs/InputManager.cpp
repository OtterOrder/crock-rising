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
	POINTS mousePt;
	
	m_MouseOldPosition = m_MousePosition;

	switch( uMsg )
	{
		case WM_MOUSEMOVE:
			mousePt				= MAKEPOINTS( lParam );
			m_MousePosition.x	= mousePt.x;
			m_MousePosition.y	= mousePt.y;
			//break;
		
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

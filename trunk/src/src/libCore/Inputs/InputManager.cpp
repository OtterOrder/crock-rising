#include "InputManager.h"

//******************************************************************

/***********************************************************
 * Fonction de rappel des événements.
 * Voir MSDN > WindowProc pour plus d'infos.
 * @param[in]	hWnd	: handle de la fenêtre
 * @param[in]	uMsg	: message
 * @param[in]	wParam	: infos supplémentaires sur le message
 * @param[in]	lParam	: infos supplémentaires sur le message
 * @return	un code résultat
 **********************************************************/
LRESULT InputManager::EventsCallback( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	//TODO

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

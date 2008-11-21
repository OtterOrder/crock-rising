#include "InputManager.h"

using namespace std;

/***********************************************************
 * Constructeur.
 **********************************************************/
InputManager::InputManager( void )
{
	m_MouseOldPosition	= Point2f( 0.f, 0.f );
	m_MousePosition		= Point2f( 0.f, 0.f );

	//TODO
}


/***********************************************************
 * Vérifie si la touche est appuyée.
 * @param[in]	key	: touche à vérifier
 * @return	true si la touche est appuyée, false sinon
 **********************************************************/
bool InputManager::IsKeyTriggered( int key ) const
{
	list< Item >::const_iterator it;
	
	it = m_Keys.begin();
	while( it != m_Keys.end() )
	{
		if( it->m_Code == key )
		{
			if( it->m_State == ITEM_TRIGGERED )
				return true;
			break;
		}
		++it;
	}
	return false;
}

/***********************************************************
 * Vérifie si la touche est maintenue appuyée.
 * @param[in]	key	: touche à vérifier
 * @return	true si la touche est maintenue appuyée,
 *			false sinon
 **********************************************************/
bool InputManager::IsKeyPressed( int key ) const
{
	//TODO
	
	return false;
}

/***********************************************************
 * Vérifie si la touche est relachée.
 * @param[in]	key	: touche à vérifier
 * @return	true si la touche est relachée, false sinon
 **********************************************************/
bool InputManager::IsKeyReleased( int key ) const
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
 * Donne le vecteur décrit par la souris depuis le dernier
 * tour moteur.
 * @return	vecteur de la souris
 **********************************************************/
Vector2f InputManager::GetMouseVector( void ) const
{
	return m_MousePosition - m_MouseOldPosition;
}


/***********************************************************
 * Update (méthode à appeler à chaque tour moteur).
 **********************************************************/
void InputManager::Update( void )
{
	list< Item >::iterator it;

	// Update de la liste des touches
	m_Keys.remove_if( InputManager::IsItemReleased );
	
	it = m_Keys.begin();
	while( it != m_Keys.end() )
	{
		if( it->m_State == ITEM_TRIGGERED )
			it->m_State = ITEM_PRESSED;
		++it;
	}

	m_MouseOldPosition = m_MousePosition;
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
	Item	item;
	POINTS	mouseCoordinates;
	list< Item >::iterator it;

	switch( uMsg )
	{
		// Touche du clavier enfoncée
		case WM_KEYDOWN:
			
			if( !(lParam & (1<<30)) )
			{
				item.m_Code		= (int)wParam;
				item.m_State	= ITEM_TRIGGERED;
				m_Keys.push_front( item );
			}
			break;

		// Touche du clavier relachée
		case WM_KEYUP:
			
			it = m_Keys.begin();
			while( it != m_Keys.end() )
			{
				if( it->m_Code == (int)wParam )
				{
					it->m_State = ITEM_RELEASED;
					break;
				}
				++it;
			}
			break;

		// Mouvement de la souris
		case WM_MOUSEMOVE:
			mouseCoordinates	= MAKEPOINTS( lParam );
			m_MousePosition.x	= (float)mouseCoordinates.x;
			m_MousePosition.y	= (float)mouseCoordinates.y;
			break;
			
		// ..
		case WM_LBUTTONDOWN:
			//TODO
			break;

	}
	return S_OK;
}


/***********************************************************
 * .
 * @param[in]	key		:
 * @param[in]	state	:
 * @return	
 **********************************************************/
bool InputManager::IsKeyStated( int key, ItemState state ) const
{
	list< Item >::const_iterator it = m_Keys.begin();
	while( it != m_Keys.end() )
	{
		if( it->m_Code == key )
		{
			if( it->m_State == state )
				return true;
			break;
		}
		++it;
	}
	return false;
}

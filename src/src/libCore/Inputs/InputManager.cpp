#include "InputManager.h"

using namespace std;

/***********************************************************
 * Constructeur.
 **********************************************************/
InputManager::InputManager( void )
{
	m_MouseOldPosition	= Point2f( 0.f, 0.f );
	m_MousePosition		= Point2f( 0.f, 0.f );
}


/***********************************************************
 * Vérifie si la touche est appuyée.
 * @param[in]	key	: touche à vérifier
 * @return	true si la touche est appuyée, false sinon
 **********************************************************/
bool InputManager::IsKeyTriggered( int key ) const
{
	return IsKeyStated( key, ITEM_TRIGGERED );
}

/***********************************************************
 * Vérifie si la touche est maintenue appuyée.
 * @param[in]	key	: touche à vérifier
 * @return	true si la touche est maintenue appuyée,
 *			false sinon
 **********************************************************/
bool InputManager::IsKeyPressed( int key ) const
{
	return IsKeyStated( key, ITEM_PRESSED );
}

/***********************************************************
 * Vérifie si la touche est relachée.
 * @param[in]	key	: touche à vérifier
 * @return	true si la touche est relachée, false sinon
 **********************************************************/
bool InputManager::IsKeyReleased( int key ) const
{
	return IsKeyStated( key, ITEM_RELEASED );
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
	UpdateItemsList( m_Keys );
	UpdateItemsList( m_MouseButtons );

	m_MouseOldPosition = m_MousePosition;
}

/***********************************************************
 * Update une liste l'items.
 * @param[out]	itemsList	: liste d'items
 **********************************************************/
void InputManager::UpdateItemsList( list< Item > &itemsList )
{
	if( itemsList.empty() )
		return;
	
	list< Item >::iterator it;
	
	// On supprime les items relachés
	itemsList.remove_if( InputManager::IsItemReleased );
	
	it = itemsList.begin();
	while( it != itemsList.end() )
	{
		if( it->m_State == ITEM_TRIGGERED )
			it->m_State = ITEM_PRESSED;
		++it;
	}
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
 * Vérifie si la touche est dans l'état spécifié.
 * @param[in]	key		: touche
 * @param[in]	state	: état
 * @return	true si la touche est dans l'état spécifié,
 *			false sinon
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

/***********************************************************
 * Vérifie si le bouton de la souris est dans l'état spécifié.
 * @param[in]	button	: bouton de la souris
 * @param[in]	state	: état
 * @return	true si le bouton est dans l'état spécifié,
 *			false sinon
 **********************************************************/
bool InputManager::IsMouseStated( int button, ItemState state ) const
{
	list< Item >::const_iterator it = m_MouseButtons.begin();
	while( it != m_MouseButtons.end() )
	{
		if( it->m_Code == button )
		{
			if( it->m_State == state )
				return true;
			break;
		}
		++it;
	}
	return false;
}

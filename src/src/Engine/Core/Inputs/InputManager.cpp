#include "InputManager.h"

using namespace std;

/***********************************************************
 * Constructeur.
 **********************************************************/
InputManager::InputManager( void )
{
	//ShowCursor( false );

	m_MouseOffset		= Point2f( 0.f, 0.f );
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
	return IsItemStated( key, ITEM_TRIGGERED, TYPE_KEY );
}

/***********************************************************
 * Vérifie si la touche est maintenue appuyée.
 * @param[in]	key	: touche à vérifier
 * @return	true si la touche est maintenue appuyée,
 *			false sinon
 **********************************************************/
bool InputManager::IsKeyPressed( int key ) const
{
	return IsItemStated( key, ITEM_PRESSED, TYPE_KEY );
}

/***********************************************************
 * Vérifie si la touche est relachée.
 * @param[in]	key	: touche à vérifier
 * @return	true si la touche est relachée, false sinon
 **********************************************************/
bool InputManager::IsKeyReleased( int key ) const
{
	return IsItemStated( key, ITEM_RELEASED, TYPE_KEY );
}


/***********************************************************
 * Vérifie si le bouton de la souris est appuyé.
 * @param[in]	button	: bouton à vérifier
 * @return	true si le bouton est appuyé, false sinon
 **********************************************************/
bool InputManager::IsMouseTriggered( int button ) const
{
	return IsItemStated( button, ITEM_TRIGGERED, TYPE_MOUSE );
}

/***********************************************************
 * Vérifie si le bouton de la souris est maintenu appuyé.
 * @param[in]	button	: bouton à vérifier
 * @return	true si le bouton est maintenu appuyé,
 *			false sinon
 **********************************************************/
bool InputManager::IsMousePressed( int button ) const
{
	return IsItemStated( button, ITEM_PRESSED, TYPE_MOUSE );
}

/***********************************************************
 * Vérifie si le bouton de la souris est relaché.
 * @param[in]	button	: bouton à vérifier
 * @return	true si le bouton est relaché, false sinon
 **********************************************************/
bool InputManager::IsMouseReleased( int button ) const
{
	return IsItemStated( button, ITEM_RELEASED, TYPE_MOUSE );
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
	UpdateList( m_Keys );
	UpdateList( m_MouseButtons );

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
	POINTS	mouseCoordinates;
	
	switch( uMsg )
	{
		// Touche du clavier enfoncée
		case WM_KEYDOWN:
			if( !(lParam & (1<<30)) )
				TriggerItem( (int)wParam, TYPE_KEY );
			break;

		// Touche du clavier relachée
		case WM_KEYUP:
			ReleaseItem( (int)wParam, TYPE_KEY );
			break;

		// Bouton gauche de la souris
		case WM_LBUTTONDOWN:
			if( !(lParam & (1<<30)) )
				TriggerItem( MOUSE_LEFT, TYPE_MOUSE );
			break;
		case WM_LBUTTONUP:
			ReleaseItem( MOUSE_LEFT, TYPE_MOUSE );
			break;

		// Bouton droit de la souris
		case WM_RBUTTONDOWN:
			if( !(lParam & (1<<30)) )
				TriggerItem( MOUSE_RIGHT, TYPE_MOUSE );
			break;
		case WM_RBUTTONUP:
			ReleaseItem( MOUSE_RIGHT, TYPE_MOUSE );
			break;

		// Bouton du millieu de la souris
		case WM_MBUTTONDOWN:
			if( !(lParam & (1<<30)) )
				TriggerItem( MOUSE_MIDDLE, TYPE_MOUSE );
			break;
		case WM_MBUTTONUP:
			ReleaseItem( MOUSE_MIDDLE, TYPE_MOUSE );
			break;

		// Mouvement de la souris
		case WM_MOUSEMOVE:
			mouseCoordinates	= MAKEPOINTS( lParam );
			m_MousePosition.x	= (float)mouseCoordinates.x;
			m_MousePosition.y	= (float)mouseCoordinates.y;

			RECT rect;
			GetWindowRect( hWnd, &rect );
			int posX = ( rect.left + rect.right )/ 2;
			int posY = ( rect.top + rect.bottom )/ 2;

			POINT pointMouse;
			GetCursorPos( &pointMouse );

			if( pointMouse.x!=posX || pointMouse.y != posY )
			{
				m_MouseOffset.x = (FLOAT)pointMouse.x - (FLOAT)posX;
				m_MouseOffset.y = (FLOAT)pointMouse.y - (FLOAT)posY;

				SetCursorPos( posX, posY );
			}

			break;

	}
	return S_OK;
}


/***********************************************************
 * Appuie l'item.
 * @param[in]	code	: code de l'item
 * @param[in]	type	: type de l'item
 **********************************************************/
void InputManager::TriggerItem( int code, ItemType type )
{
	Item item;
	item.m_Code		= code;
	item.m_State	= ITEM_TRIGGERED;
	GetList( type )->push_front( item );
}

/***********************************************************
* Relache l'item.
* @param[in]	code	: code de l'item
* @param[in]	type	: type de l'item
**********************************************************/
void InputManager::ReleaseItem( int code, ItemType type )
{
	list< Item > *pl;
	list< Item >::iterator it;

	pl = GetList( type );
	it = pl->begin();

	while( it != pl->end() )
	{
		if( it->m_Code == code )
		{
			it->m_State = ITEM_RELEASED;
			break;
		}
		++it;
	}
}

/***********************************************************
 * Vérifie si l'item est dans l'état spécifié.
 * @param[in]	code	: code de l'item
 * @param[in]	state	: état de l'item
 * @param[in]	type	: type de l'item
 * @return	true si l'item est dans l'état spécifié,
 *			false sinon
 **********************************************************/
bool InputManager::IsItemStated( int code, ItemState state, ItemType type ) const
{
	const list< Item > *pl;
	list< Item >::const_iterator it;

	pl = GetConstList( type );
	it = pl->begin();

	while( it != pl->end() )
	{
		if( it->m_Code == code )
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
 * Update une liste l'items.
 * @param[out]	items	: liste d'items
 **********************************************************/
void InputManager::UpdateList( list< Item > &items )
{
	if( items.empty() )
		return;

	list< Item >::iterator it;

	// On supprime les items relachés
	items.remove_if( InputManager::IsItemReleased );

	it = items.begin();
	while( it != items.end() )
	{
		if( it->m_State == ITEM_TRIGGERED )
			it->m_State = ITEM_PRESSED;
		++it;
	}
}

/***********************************************************
 * Donne la liste d'items correspondante au type.
 * @param[in]	type	: type des items
 * @return	pointeur sur la bonne liste
 **********************************************************/
list< InputManager::Item >* InputManager::GetList( ItemType type )
{
	switch( type )
	{
		case TYPE_KEY:		return &m_Keys;
		case TYPE_MOUSE:	return &m_MouseButtons;
	}
	return NULL;
}

/***********************************************************
 * Donne la liste (constante) d'items correspondante au type.
 * @param[in]	type	: type des items
 * @return	pointeur sur la bonne liste
 **********************************************************/
const std::list< InputManager::Item >* InputManager::GetConstList( ItemType type ) const
{
	switch( type )
	{
		case TYPE_KEY:		return &m_Keys;
		case TYPE_MOUSE:	return &m_MouseButtons;
	}
	return NULL;
}



Point2f InputManager::GetMouseOffset( void )
{
	Point2f toReturn = m_MouseOffset;

	m_MouseOffset.x = 0.f;
	m_MouseOffset.y = 0.f;

	return toReturn;
}
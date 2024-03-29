#include "InputManager.h"

#include "Renderer/Renderer.h"
#include "Objects/2D/Sprite.h"

using namespace std;

/***********************************************************
 * Constructeur.
 **********************************************************/
InputManager::InputManager( void )
{
	InitMouseWheelDelta();

	m_MousePosition			= Point2f( 0.f, 0.f );
	m_MouseVector			= Vector2f( 0.f, 0.f );
	m_IsMouseMoved			= false;
	m_IsMouseHeldAtCenter	= false;
	m_pCursor				= NULL;
}

/***********************************************************
 * Destructeur.
 **********************************************************/
InputManager::~InputManager( void )
{
	if( m_pCursor )
		delete m_pCursor;
}

/***********************************************************
 * V�rifie si la touche est appuy�e.
 * @param[in]	key	: touche � v�rifier
 * @return	true si la touche est appuy�e, false sinon
 **********************************************************/
bool InputManager::IsKeyTriggered( int key ) const
{
	return IsItemStated( key, ITEM_TRIGGERED, TYPE_KEY );
}

/***********************************************************
 * V�rifie si la touche est maintenue appuy�e.
 * @param[in]	key	: touche � v�rifier
 * @return	true si la touche est maintenue appuy�e,
 *			false sinon
 **********************************************************/
bool InputManager::IsKeyPressed( int key ) const
{
	return IsItemStated( key, ITEM_PRESSED, TYPE_KEY );
}

/***********************************************************
 * V�rifie si la touche est relach�e.
 * @param[in]	key	: touche � v�rifier
 * @return	true si la touche est relach�e, false sinon
 **********************************************************/
bool InputManager::IsKeyReleased( int key ) const
{
	return IsItemStated( key, ITEM_RELEASED, TYPE_KEY );
}


/***********************************************************
 * V�rifie si le bouton de la souris est appuy�.
 * @param[in]	button	: bouton � v�rifier
 * @return	true si le bouton est appuy�, false sinon
 **********************************************************/
bool InputManager::IsMouseTriggered( int button ) const
{
	return IsItemStated( button, ITEM_TRIGGERED, TYPE_MOUSE );
}

/***********************************************************
 * V�rifie si le bouton de la souris est maintenu appuy�.
 * @param[in]	button	: bouton � v�rifier
 * @return	true si le bouton est maintenu appuy�,
 *			false sinon
 **********************************************************/
bool InputManager::IsMousePressed( int button ) const
{
	return IsItemStated( button, ITEM_PRESSED, TYPE_MOUSE );
}

/***********************************************************
 * V�rifie si le bouton de la souris est relach�.
 * @param[in]	button	: bouton � v�rifier
 * @return	true si le bouton est relach�, false sinon
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
 * Donne le vecteur d�crit par la souris depuis le dernier
 * tour moteur.
 * @return	vecteur de la souris
 **********************************************************/
Vector2f InputManager::GetMouseVector( void ) const
{
	return m_MouseVector;
}

//**********************************************************
// V�rouille ou non la souris au centre de l'�cran.
// @param[in]	isHeld : vrai = souris v�rouill�e au centre
//**********************************************************
void InputManager::HoldMouseAtCenter( bool isHeld )
{
	m_IsMouseHeldAtCenter = isHeld;
}

//**********************************************************
// Rend le curseur par d�faut du syst�me visible ou non.
// @param[in]	isShown : vrai = curseur visible
//**********************************************************
void InputManager::ShowOSCursor( bool isShown )
{
	ShowCursor( isShown );
}

//**********************************************************
// Set le sprite � utiliser comme curseur.
// @param[in]	pCursor : Pointeur sur le sprite
//**********************************************************
void InputManager::SetCursorSprite( Sprite *pCursor )
{
	if( m_pCursor )
	{
		delete m_pCursor;
		m_pCursor = NULL;
	}
	m_pCursor = pCursor;

	if( m_pCursor )
	{
		m_pCursor->SetPriority( 1 );
	}
}

//**********************************************************
// Donne le sprite du curseur.
// @return	Pointeur sur le sprite
//**********************************************************
Sprite* const InputManager::GetCursorSprite( void ) const
{
	return m_pCursor;
}

/***********************************************************
 * Update (m�thode � appeler � chaque tour moteur).
 **********************************************************/
void InputManager::Update( void )
{
	UpdateList( m_Keys );
	UpdateList( m_MouseButtons );

	if( m_IsMouseMoved )
	{
		m_MouseVector	= Vector2f( 0.f, 0.f );
		m_IsMouseMoved	= false;
	}
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
			if( !(lParam & (1<<30)) )
				TriggerItem( (int)wParam, TYPE_KEY );
			break;

		// Touche du clavier relach�e
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
		{
			Point2f oldMousePosition;

			m_IsMouseMoved = true;
			oldMousePosition = m_MousePosition;

			if( !m_IsMouseHeldAtCenter )
			{
				POINTS mouseWndPosition;
				mouseWndPosition	= MAKEPOINTS( lParam );
				m_MousePosition.x	= (float)mouseWndPosition.x;
				m_MousePosition.y	= (float)mouseWndPosition.y;
				m_MouseVector		= m_MousePosition - oldMousePosition;
			}
			else
			{
				// La souris doit �tre coinc�e au centre..

				int centerX, centerY;
				POINT mouseAbsPosition;
				RECT window;
				
				GetCursorPos( &mouseAbsPosition );
				GetWindowRect( hWnd, &window );
				centerX = ( window.left + window.right ) / 2;
				centerY = ( window.top + window.bottom ) / 2;

				// Coordonn�es de la souris dans l'espace de la fen�tre
				m_MousePosition.x = (float)mouseAbsPosition.x - window.left;
				m_MousePosition.y = (float)mouseAbsPosition.y - window.top;
				
				if( mouseAbsPosition.x != centerX	||
					mouseAbsPosition.y != centerY	)
				{
					SetCursorPos( centerX, centerY );
					
					// Vecteur du d�placement
					m_MouseVector.x = (float)mouseAbsPosition.x - centerX;
					m_MouseVector.y = (float)mouseAbsPosition.y - centerY;
				}
			}
			if( m_pCursor )
			{
				// Position du curseur
				m_pCursor->SetPosition( m_MousePosition );
			}
			break;
		}

		case WM_MOUSEWHEEL: 
		{
            // Mise � jour variables d'�tat
			int acc=(short)HIWORD(wParam);
			if(acc<1200.0f)
			{
				int sensibilite = 120;
				m_nMouseWheelDelta = (short)HIWORD(wParam) / sensibilite;

			}
            break;
		}
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
 * V�rifie si l'item est dans l'�tat sp�cifi�.
 * @param[in]	code	: code de l'item
 * @param[in]	state	: �tat de l'item
 * @param[in]	type	: type de l'item
 * @return	true si l'item est dans l'�tat sp�cifi�,
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

	// On supprime les items relach�s
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

int InputManager::GetMouseWheelDelta()
{ 
	return m_nMouseWheelDelta; 
}

void InputManager::InitMouseWheelDelta()
{ 
	m_nMouseWheelDelta = 0; 
}
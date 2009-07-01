#ifndef		_InputManager_H
#define		_InputManager_H

//Define pour utilisé plus de chose (WM_MOUSEWHEEL entre autre) et en jarté d'autre

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclure les en-têtes Windows rarement utilisés
#endif

// Modifiez les définitions suivantes si vous devez cibler une plate-forme avant celles spécifiées ci-dessous.
// Reportez-vous à MSDN pour obtenir les dernières informations sur les valeurs correspondantes pour les différentes plates-formes.
#ifndef WINVER				// Autorise l'utilisation des fonctionnalités spécifiques à Windows XP ou version ultérieure.
#define WINVER 0x0501		// Attribuez la valeur appropriée à cet élément pour cibler d'autres versions de Windows.
#endif

#ifndef _WIN32_WINNT		// Autorise l'utilisation des fonctionnalités spécifiques à Windows XP ou version ultérieure.                   
#define _WIN32_WINNT 0x0501	// Attribuez la valeur appropriée à cet élément pour cibler d'autres versions de Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Autorise l'utilisation des fonctionnalités spécifiques à Windows 98 ou version ultérieure.
#define _WIN32_WINDOWS 0x0410 // Attribuez la valeur appropriée à cet élément pour cibler Windows Me ou version ultérieure.
#endif



//******************************************************************

#include	<list>

#include	"Core/Singleton.h"
#include	"Core/Types/Vector.h"

//******************************************************************

enum MouseButton
{
	MOUSE_LEFT,
	MOUSE_RIGHT,
	MOUSE_MIDDLE,
	//MOUSE_UP,
	//MOUSE_DOWN
};

//******************************************************************

class Sprite;

class InputManager : public Singleton< InputManager >
{
	// Indispensable car Singleton doit pouvoir accéder aux
	// constructeur/destructeur, qui sont en protected.
	friend class Singleton< InputManager >;


public:

	// =========================================================
	// Méthodes publiques

	// Note: pour vérifier une lettre, on envoie directement
	// le caractère (par exemple 'A'). Pour les touches spéciales,
	// on utilise les defines de WINAPI qui commencent par VK_
	// (en général, sinon voir dans winuser.h).
	
	bool		IsKeyTriggered		( int key ) const;		// Vérifie si la touche est appuyée
	bool		IsKeyPressed		( int key ) const;		// Vérifie si la touche est maintenue appuyée
	bool		IsKeyReleased		( int key ) const;		// Vérifie si la touche est relachée

	bool		IsMouseTriggered	( int button ) const;	// Vérifie si le bouton de la souris est appuyé
	bool		IsMousePressed		( int button ) const;	// Vérifie si le bouton de la souris est maintenu appuyé
	bool		IsMouseReleased		( int button ) const;	// Vérifie si le bouton de la souris est relaché
	
	Point2f		GetMousePosition	( void ) const;			// Donne la position de la souris
	Vector2f	GetMouseVector		( void ) const;			// Donne le vecteur de la souris
	int			GetMouseWheelDelta	( void );

	void			HoldMouseAtCenter	( bool isHeld );
	void			ShowOSCursor		( bool isShown );
	void			SetCursorSprite		( Sprite *pCursor );
	Sprite* const	GetCursorSprite		( void ) const;

	void		Update				( void );				// Update
	void		InitMouseWheelDelta( void );

	// Fonction de rappel des événements
	LRESULT CALLBACK	EventsCallback	( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


protected:

	// =========================================================
	// Enum et stuctures protégées

	enum ItemType
	{
		TYPE_KEY,
		TYPE_MOUSE
	};
	
	enum ItemState
	{
		ITEM_TRIGGERED,
		ITEM_PRESSED,
		ITEM_RELEASED
	};
	
	struct Item // Désigne une touche ou un bouton
	{
		int			m_Code;			// Code de l'item
		ItemState	m_State;		// Etat de l'item
	};


	// =========================================================
	// Données protégées

	std::list< Item >	m_Keys;					// Liste des touches
	std::list< Item >	m_MouseButtons;			// Liste des boutons de souris
	
	Point2f				m_MousePosition;		// Position de la souris
	Vector2f			m_MouseVector;			// Vecteur déplacement de la souris
	bool				m_IsMouseMoved;			// Si la souris vient juste de bouger
	bool				m_IsMouseHeldAtCenter;	// Si la souris est vérouillée au centre

	int                 m_nMouseWheelDelta;

	Sprite				*m_pCursor;
	
	// =========================================================
	// Méthodes protégées

	InputManager					( void );								// Constructeur
	virtual ~InputManager			( void );								// Destructeur

	void		TriggerItem			( int code, ItemType type );			// Appuie l'item
	void		ReleaseItem			( int code, ItemType type );			// Relache l'item
	
	bool		IsItemStated		( int code, ItemState state, ItemType type ) const;// Vérifie si l'item est dans l'état spécifié.

	void						UpdateList		( std::list< Item > &items );	// Update une liste l'items
	std::list< Item >*			GetList			( ItemType type );				// Donne la liste d'items correspondante au type
	const std::list< Item >*	GetConstList	( ItemType type ) const;		// Donne la liste d'items correspondante au type

	
	/***********************************************************
	 * Vérifie si l'item est relaché. Utile pour l'update
	 * des liste d'items.
	 **********************************************************/
	static bool	IsItemReleased( const Item &item )
	{
		return item.m_State == ITEM_RELEASED;
	}

};


//******************************************************************
#endif		// _InputManager_H

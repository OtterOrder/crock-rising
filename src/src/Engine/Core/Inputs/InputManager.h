#ifndef		_InputManager_H
#define		_InputManager_H

//Define pour utilis� plus de chose (WM_MOUSEWHEEL entre autre) et en jart� d'autre

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclure les en-t�tes Windows rarement utilis�s
#endif

// Modifiez les d�finitions suivantes si vous devez cibler une plate-forme avant celles sp�cifi�es ci-dessous.
// Reportez-vous � MSDN pour obtenir les derni�res informations sur les valeurs correspondantes pour les diff�rentes plates-formes.
#ifndef WINVER				// Autorise l'utilisation des fonctionnalit�s sp�cifiques � Windows�XP ou version ult�rieure.
#define WINVER 0x0501		// Attribuez la valeur appropri�e � cet �l�ment pour cibler d'autres versions de Windows.
#endif

#ifndef _WIN32_WINNT		// Autorise l'utilisation des fonctionnalit�s sp�cifiques � Windows�XP ou version ult�rieure.                   
#define _WIN32_WINNT 0x0501	// Attribuez la valeur appropri�e � cet �l�ment pour cibler d'autres versions de Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Autorise l'utilisation des fonctionnalit�s sp�cifiques � Windows�98 ou version ult�rieure.
#define _WIN32_WINDOWS 0x0410 // Attribuez la valeur appropri�e � cet �l�ment pour cibler Windows�Me ou version ult�rieure.
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
	// Indispensable car Singleton doit pouvoir acc�der aux
	// constructeur/destructeur, qui sont en protected.
	friend class Singleton< InputManager >;


public:

	// =========================================================
	// M�thodes publiques

	// Note: pour v�rifier une lettre, on envoie directement
	// le caract�re (par exemple 'A'). Pour les touches sp�ciales,
	// on utilise les defines de WINAPI qui commencent par VK_
	// (en g�n�ral, sinon voir dans winuser.h).
	
	bool		IsKeyTriggered		( int key ) const;		// V�rifie si la touche est appuy�e
	bool		IsKeyPressed		( int key ) const;		// V�rifie si la touche est maintenue appuy�e
	bool		IsKeyReleased		( int key ) const;		// V�rifie si la touche est relach�e

	bool		IsMouseTriggered	( int button ) const;	// V�rifie si le bouton de la souris est appuy�
	bool		IsMousePressed		( int button ) const;	// V�rifie si le bouton de la souris est maintenu appuy�
	bool		IsMouseReleased		( int button ) const;	// V�rifie si le bouton de la souris est relach�
	
	Point2f		GetMousePosition	( void ) const;			// Donne la position de la souris
	Vector2f	GetMouseVector		( void ) const;			// Donne le vecteur de la souris
	int			GetMouseWheelDelta	( void );

	void			HoldMouseAtCenter	( bool isHeld );
	void			ShowOSCursor		( bool isShown );
	void			SetCursorSprite		( Sprite *pCursor );
	Sprite* const	GetCursorSprite		( void ) const;

	void		Update				( void );				// Update
	void		InitMouseWheelDelta( void );

	// Fonction de rappel des �v�nements
	LRESULT CALLBACK	EventsCallback	( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


protected:

	// =========================================================
	// Enum et stuctures prot�g�es

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
	
	struct Item // D�signe une touche ou un bouton
	{
		int			m_Code;			// Code de l'item
		ItemState	m_State;		// Etat de l'item
	};


	// =========================================================
	// Donn�es prot�g�es

	std::list< Item >	m_Keys;					// Liste des touches
	std::list< Item >	m_MouseButtons;			// Liste des boutons de souris
	
	Point2f				m_MousePosition;		// Position de la souris
	Vector2f			m_MouseVector;			// Vecteur d�placement de la souris
	bool				m_IsMouseMoved;			// Si la souris vient juste de bouger
	bool				m_IsMouseHeldAtCenter;	// Si la souris est v�rouill�e au centre

	int                 m_nMouseWheelDelta;

	Sprite				*m_pCursor;
	
	// =========================================================
	// M�thodes prot�g�es

	InputManager					( void );								// Constructeur
	virtual ~InputManager			( void );								// Destructeur

	void		TriggerItem			( int code, ItemType type );			// Appuie l'item
	void		ReleaseItem			( int code, ItemType type );			// Relache l'item
	
	bool		IsItemStated		( int code, ItemState state, ItemType type ) const;// V�rifie si l'item est dans l'�tat sp�cifi�.

	void						UpdateList		( std::list< Item > &items );	// Update une liste l'items
	std::list< Item >*			GetList			( ItemType type );				// Donne la liste d'items correspondante au type
	const std::list< Item >*	GetConstList	( ItemType type ) const;		// Donne la liste d'items correspondante au type

	
	/***********************************************************
	 * V�rifie si l'item est relach�. Utile pour l'update
	 * des liste d'items.
	 **********************************************************/
	static bool	IsItemReleased( const Item &item )
	{
		return item.m_State == ITEM_RELEASED;
	}

};


//******************************************************************
#endif		// _InputManager_H

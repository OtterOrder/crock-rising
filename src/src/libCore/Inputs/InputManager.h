#ifndef		_InputManager_H
#define		_InputManager_H

//******************************************************************

#include	<libCore/Singleton/Singleton.h>
#include	<libCore/Types/Vector.h>

#include	<windows.h>
#include	<list>

//******************************************************************

class InputManager : public Singleton< InputManager >
{
	// Indispensable car Singleton doit pouvoir accéder aux
	// constructeur/destructeur, qui sont en protected.
	friend class Singleton< InputManager >;

public:

	// =========================================================
	// Méthodes publiques

	// Interface pour le clavier
	// Note: pour vérifier une lettre, on envoie directement
	// le caractère (par exemple 'A'). Pour les touches spéciales,
	// on utilise les defines de WINAPI qui commencent par VK_
	// (en général, sinon voir dans winuser.h).
	bool				IsKeyTriggered	( int key );		// Vérifie si la touche est appuyée
	bool				IsKeyPressed	( int key );		// Vérifie si la touche est maintenue appuyée
	bool				IsKeyReleased	( int key );		// Vérifie si la touche est relachée

	// Interface pour la souris
	Vector3f			GetMouseVector	( void ) const;		// Donne le vecteur de la souris

	bool				IsMouseMoving	( void ) const;


	LRESULT CALLBACK	EventsCallback	( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	// Fonction de rappel des événements


protected:

	// =========================================================
	// Enum et stuctures privées

	enum KeyState
	{
		KEY_TRIGGERED,
		KEY_PRESSED,
		KEY_RELEASED

	};

	struct Key
	{
		UINT_PTR	m_KeyCode;			// Code de la touche
		KeyState	m_State;			// Etat de la touche

	};


	// =========================================================
	// Données privées

	std::list< Key >			m_Keys;

	Point3f						m_LastMousePosition;	// Dernière position de la souris
	Vector3f					m_MouseVector;			// Vecteur de la souris

	int							m_MouseX;
	int							m_MouseY;

	
	// =========================================================
	// Méthodes privées

	InputManager					( void );			// Constructeur
	virtual ~InputManager			( void ){}			// Destructeur

};


//******************************************************************
#endif		// _InputManager_H

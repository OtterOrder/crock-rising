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
	Point2f				GetMousePosition( void ) const;		// Donne la position de la souris
	Vector2f			GetMouseVector	( void ) const;		// Donne le vecteur de la souris

	bool				IsMouseMoving	( void ) const;


	LRESULT CALLBACK	EventsCallback	( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	// Fonction de rappel des événements


protected:

	// =========================================================
	// Enum et stuctures protégées

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
	// Données protégées

	Point2f				m_MouseOldPosition;		// Ancienne position de la souris
	Point2f				m_MousePosition;		// Position de la souris

	bool				m_IsMouseMoving;		// La souris bouge ?

	
	// =========================================================
	// Méthodes protégées

	InputManager					( void );			// Constructeur
	virtual ~InputManager			( void ){}			// Destructeur

};


//******************************************************************
#endif		// _InputManager_H

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
	// Indispensable car Singleton doit pouvoir acc�der aux
	// constructeur/destructeur, qui sont en protected.
	friend class Singleton< InputManager >;

public:

	// =========================================================
	// M�thodes publiques

	// Interface pour le clavier
	// Note: pour v�rifier une lettre, on envoie directement
	// le caract�re (par exemple 'A'). Pour les touches sp�ciales,
	// on utilise les defines de WINAPI qui commencent par VK_
	// (en g�n�ral, sinon voir dans winuser.h).
	bool				IsKeyTriggered	( int key );		// V�rifie si la touche est appuy�e
	bool				IsKeyPressed	( int key );		// V�rifie si la touche est maintenue appuy�e
	bool				IsKeyReleased	( int key );		// V�rifie si la touche est relach�e

	// Interface pour la souris
	Vector3f			GetMouseVector	( void ) const;		// Donne le vecteur de la souris

	bool				IsMouseMoving	( void ) const;


	LRESULT CALLBACK	EventsCallback	( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	// Fonction de rappel des �v�nements


protected:

	// =========================================================
	// Enum et stuctures priv�es

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
	// Donn�es priv�es

	std::list< Key >			m_Keys;

	Point3f						m_LastMousePosition;	// Derni�re position de la souris
	Vector3f					m_MouseVector;			// Vecteur de la souris

	int							m_MouseX;
	int							m_MouseY;

	
	// =========================================================
	// M�thodes priv�es

	InputManager					( void );			// Constructeur
	virtual ~InputManager			( void ){}			// Destructeur

};


//******************************************************************
#endif		// _InputManager_H

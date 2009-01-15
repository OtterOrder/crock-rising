#ifndef		_System_H
#define		_System_H

#include	<windows.h>
#include	<d3d9.h>

#include	"Core/Singleton.h"

//******************************************************************

class Game;
class Renderer;
class InputManager;

class System : public Singleton< System >
{
	// Indispensable car Singleton doit pouvoir acc�der aux
	// constructeur/destructeur, qui sont en protected.
	friend class Singleton< System >;

public:

	// =========================================================
	// M�thodes publiques

	void	Initialize	( void );	// Initialise l'appli
	int		MainLoop	( void );	// Boucle principale de l'appli

	static LRESULT CALLBACK	EventsCallback( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );// Fonction de rappel des �v�nements


protected:

	// =========================================================
	// M�thodes prot�g�es
	
	System				( void );		// Constructeur
	~System				( void );		// Destructeur
	
	HRESULT	InitWindow	( void );		// Initialisation fen�tre windows

	
	// =========================================================
	// Donn�es prot�g�es
	
	static bool		s_IsInitialized;	// Si le System est initialis�

	Game*			m_Game;				// Le jeu
	Renderer*		m_Renderer;			// Moteur de rendu
	InputManager*	m_InputManager;		// Gestionnaire d'inputs

	HINSTANCE		m_Instance;			// Instance de l'application
	HICON			m_Icon;

};

//******************************************************************
#endif		// _System_H

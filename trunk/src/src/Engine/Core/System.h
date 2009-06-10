#ifndef		_SYSTEM_H_
#define		_SYSTEM_H_

//******************************************************************

#ifndef		NOMINMAX
#define		NOMINMAX
#endif

#include	<windows.h>
#include	"Core/Singleton.h"

//******************************************************************

class Time;

class System : public Singleton< System >
{
	// Indispensable car Singleton doit pouvoir accéder aux
	// constructeur/destructeur, qui sont en protected.
	friend class Singleton< System >;

public:

	// =========================================================
	// Méthodes publiques

	int		MainLoop	( void );	// Boucle principale de l'appli
	
	Time* const GetTime() const;	// Donne le gestionnaire de temps

	static LRESULT CALLBACK	EventsCallback( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );// Fonction de rappel des événements


protected:

	// =========================================================
	// Méthodes protégées
	
	System				( void );		// Constructeur
	virtual ~System		( void );		// Destructeur
	
	HRESULT	InitWindow	( void );		// Initialisation fenêtre windows

	
	// =========================================================
	// Données protégées
	
	HINSTANCE		m_Instance;			// Instance de l'application
	HICON			m_Icon;
	Time			*m_Time;			// Gestion du temps

};

//******************************************************************
#endif		//_SYSTEM_H_

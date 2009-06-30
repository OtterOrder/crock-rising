#ifndef		_SYSTEM_H_
#define		_SYSTEM_H_

//******************************************************************

#ifndef		NOMINMAX
#define		NOMINMAX
#endif

#include	<windows.h>
#include	"Core/Singleton.h"

//******************************************************************

#define		FULLS_DEFAULT_WIDTH			800
#define		FULLS_DEFAULT_HEIGHT		600

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

	void	Init		( void );	// Initialise le System
	void	Shutdown	( void );	// Termine l'appli
	int		MainLoop	( void );	// Boucle principale de l'appli
	
	void	SetFullScreen( bool fullScreen, int width = FULLS_DEFAULT_WIDTH, int height = FULLS_DEFAULT_HEIGHT );

	Time* const GetTime() const;	// Donne le gestionnaire de temps

	static LRESULT CALLBACK	EventsCallback( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );// Fonction de rappel des événements


protected:

	// =========================================================
	// Méthodes protégées
	
	System					( void );	// Constructeur
	virtual ~System			( void );	// Destructeur
	
	HRESULT	InitRenderer	( void );	// Initialise le Renderer

	
	// =========================================================
	// Données protégées
	
	HINSTANCE		m_Instance;			// Instance de l'application
	HICON			m_Icon;
	Time			*m_Time;			// Gestion du temps

	bool			m_FullScreen;
	int				m_FullScreenWidth;
	int				m_FullScreenHeight;

};

//******************************************************************
#endif		//_SYSTEM_H_

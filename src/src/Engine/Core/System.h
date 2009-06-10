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
	// Indispensable car Singleton doit pouvoir acc�der aux
	// constructeur/destructeur, qui sont en protected.
	friend class Singleton< System >;

public:

	// =========================================================
	// M�thodes publiques

	int		MainLoop	( void );	// Boucle principale de l'appli
	
	Time* const GetTime() const;	// Donne le gestionnaire de temps

	static LRESULT CALLBACK	EventsCallback( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );// Fonction de rappel des �v�nements


protected:

	// =========================================================
	// M�thodes prot�g�es
	
	System				( void );		// Constructeur
	virtual ~System		( void );		// Destructeur
	
	HRESULT	InitWindow	( void );		// Initialisation fen�tre windows

	
	// =========================================================
	// Donn�es prot�g�es
	
	HINSTANCE		m_Instance;			// Instance de l'application
	HICON			m_Icon;
	Time			*m_Time;			// Gestion du temps

};

//******************************************************************
#endif		//_SYSTEM_H_

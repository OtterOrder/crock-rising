#ifndef		_2D_ESCMENU_H_
#define		_2D_ESCMENU_H_

//******************************************************************

#include	<Objects/2D/Quad.h>
#include	"ButtonText.h"

//******************************************************************

class EscMenu : protected Quad
{
public:

	enum Link
	{
		BACK = 0,
		MAINMENU,
		QUITGAME,

		NB_LINKS
	};

	EscMenu( bool isMouseFree = false );
	virtual ~EscMenu();

	void Init();
	virtual void Update();

	void Open();
	void Close();

protected:

	enum State
	{
		CLOSED,
		OPENING,
		OPENED,
		CLOSING
	};

	// Bouton du menu ESC
	struct EscButton : public ButtonText
	{
		Link	linkID;
		EscMenu	*pMenu;

		EscButton( Link _linkID, EscMenu *_pMenu = NULL );
		void OnClic();
	};

	State		m_State;			// Etat courant du menu
	bool		m_IsMouseFree;		// Si la souris doit être libre en dehors du menu
	float		m_AppTimer;			// Timer d'apparition/disparition
	
	EscButton	*m_Menu[NB_LINKS];	// Boutons du menu

	void SetAlpha( float alpha );

};

//******************************************************************
#endif		//_2D_ESCMENU_H_

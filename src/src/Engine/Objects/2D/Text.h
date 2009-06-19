#ifndef		_TEXT_H_
#define		_TEXT_H_

//******************************************************************

#include	<string>
#include	"SceneObject2D.h"

//******************************************************************

class Text : public SceneObject2D
{
public:

	//-- Méthodes publiques
	
	Text( const std::string &text = "" );
	virtual ~Text();

	// Affichage
	virtual void Draw();

	// Propriétés du texte
	void SetText( const std::string &text );
	void SetFont( const std::string &fontName );
	void SetFontHeight( u32 height );
	void SetFontWeight( u32 weight );

	// Gestion des entités DirectX
	virtual void OnCreateDevice();
	virtual void OnResetDevice();
	virtual void OnLostDevice();
	virtual void OnDestroyDevice();
	virtual bool IsDrawable() const;

protected:

	//-- Données protégées

	std::string		m_Text;
	D3DXFONT_DESC	m_Desc;
	LPD3DXFONT		m_Font;

};

//******************************************************************
#endif		//_TEXT_H_

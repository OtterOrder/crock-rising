#ifndef		_TEXT_H_
#define		_TEXT_H_

//******************************************************************

#include	<string>
#include	"SceneObject2D.h"

//******************************************************************

// Note: Text utilise un objet de type ID3DXFont, par conséquent,
// les transformations d'échelle (Scale) et de rotation (Rotation)
// sont inopérantes.

class Text : public SceneObject2D
{
public:

	//-- Méthodes publiques
	
	Text( const std::string &text = "" );
	Text( const std::string &text, const std::string &fontName, u32 size, bool bold, bool italic );
	virtual ~Text();

	// Affichage
	virtual void Draw();

	// Propriétés du texte
	void SetText( const std::string &text );
	void AddText( const std::string &text );
	void SetFormat( u32 format );

	// Bulle d'affichage
	void SetBubbleSize( u32 width, u32 height );
	void SetBubbleSize( const Vector2i &size );
	void SetBubbleWidth( u32 width );
	void SetBubbleHeight( u32 height );
	void ComputeBubbleSize();

	// Collision
	virtual bool IsCollide( s32 posX, s32 posY );

	// Gestion des entités DirectX
	virtual void OnCreateDevice();
	virtual void OnResetDevice();
	virtual void OnLostDevice();
	virtual void OnDestroyDevice();
	virtual bool IsDrawable() const;

protected:

	//-- Structures protégées

	struct FontProperties
	{
		std::string		fontName;	// Nom de la fonte
		u32				size;		// Taille (px)
		bool			bold;		// Gras
		bool			italic;		// Italique
	};

	//-- Données protégées

	std::string		m_Text;				// Texte
	u32				m_Format;			// Formatage
	u32				m_BubbleWidth;		// Largeur de la bulle d'affichage
	u32				m_BubbleHeight;		// Hauteur de la bulle d'affichage
	RECT			m_Bubble;			// Bulle d'affichage
	
	FontProperties	m_FontProperties;	// Propriétés de la fonte
	LPD3DXFONT		m_Font;				// Fonte d3dx

	//-- Méthodes protégées

	void CommonInit();				// Initialisation commune aux constructeurs
	virtual void dirty_Refresh();	// Met à jour tout le bazar

};

//******************************************************************
#endif		//_TEXT_H_

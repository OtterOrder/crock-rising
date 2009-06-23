#ifndef		_TEXT_H_
#define		_TEXT_H_

//******************************************************************

#include	<string>
#include	"SceneObject2D.h"

//******************************************************************

// Note: Text utilise un objet de type ID3DXFont, par cons�quent,
// les transformations d'�chelle (Scale) et de rotation (Rotation)
// sont inop�rantes.

class Text : public SceneObject2D
{
public:

	//-- M�thodes publiques
	
	Text( const std::string &text = "" );
	Text( const std::string &text, const std::string &fontName, u32 size, bool bold, bool italic );
	virtual ~Text();

	// Affichage
	virtual void Draw();

	// Propri�t�s du texte
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

	// Gestion des entit�s DirectX
	virtual void OnCreateDevice();
	virtual void OnResetDevice();
	virtual void OnLostDevice();
	virtual void OnDestroyDevice();
	virtual bool IsDrawable() const;

protected:

	//-- Structures prot�g�es

	struct FontProperties
	{
		std::string		fontName;	// Nom de la fonte
		u32				size;		// Taille (px)
		bool			bold;		// Gras
		bool			italic;		// Italique
	};

	//-- Donn�es prot�g�es

	std::string		m_Text;				// Texte
	u32				m_Format;			// Formatage
	u32				m_BubbleWidth;		// Largeur de la bulle d'affichage
	u32				m_BubbleHeight;		// Hauteur de la bulle d'affichage
	RECT			m_Bubble;			// Bulle d'affichage
	
	FontProperties	m_FontProperties;	// Propri�t�s de la fonte
	LPD3DXFONT		m_Font;				// Fonte d3dx

	//-- M�thodes prot�g�es

	void CommonInit();				// Initialisation commune aux constructeurs
	virtual void dirty_Refresh();	// Met � jour tout le bazar

};

//******************************************************************
#endif		//_TEXT_H_

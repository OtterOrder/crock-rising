#ifndef		_TEXT_H_
#define		_TEXT_H_

//******************************************************************

#include	<string>
#include	"Object2D.h"

//******************************************************************

class Text : public Object2D
{
public:

	//-- M�thodes publiques
	
	Text( const char *text );
	virtual ~Text();

	virtual void Draw();		// Affiche l'objet

	void SetText( const char *text );

	//-- M�thodes sp�cifiques pour DirectX (utilis�es dans le Renderer)
	
	virtual void InitDxData();		// Initialise les donn�es Dx
	virtual void ClearDxData();		// Lib�re les donn�es Dx
	virtual bool IsDxReady() const;	// V�rifie si les donn�es Dx sont pr�tes

protected:

	//-- Donn�es prot�g�es

	std::string		m_Text;
	ID3DXFont		*m_pFont;
	RECT			m_Rect;

};

//******************************************************************
#endif		//_TEXT_H_

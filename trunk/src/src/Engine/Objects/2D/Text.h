#ifndef		_TEXT_H_
#define		_TEXT_H_

//******************************************************************

#include	<string>
#include	"Object2D.h"

//******************************************************************

class Text : public Object2D
{
public:

	//-- Méthodes publiques
	
	Text( const char *text );
	virtual ~Text();

	virtual void Draw();		// Affiche l'objet

	void SetText( const char *text );

	//-- Méthodes spécifiques pour DirectX (utilisées dans le Renderer)
	
	virtual void InitDxData();		// Initialise les données Dx
	virtual void ClearDxData();		// Libère les données Dx
	virtual bool IsDxReady() const;	// Vérifie si les données Dx sont prètes

protected:

	//-- Données protégées

	std::string		m_Text;
	ID3DXFont		*m_pFont;
	RECT			m_Rect;

};

//******************************************************************
#endif		//_TEXT_H_

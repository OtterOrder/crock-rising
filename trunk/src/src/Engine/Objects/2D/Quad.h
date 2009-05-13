#ifndef		_Quad_H
#define		_Quad_H

//******************************************************************

#include	"Object2D.h"

//******************************************************************

class Quad : public Object2D
{
public:

	//-- Méthodes publiques
	
	Quad( int width, int height, const Color4f &color );
	Quad( const Vector2f &size, const Color4f &color );
	virtual ~Quad();

	virtual void Draw();		// Affiche l'objet

	//-- Méthodes spécifiques pour DirectX (utilisées dans le Renderer)
	
	virtual void InitDxData();		// Initialise les données Dx
	virtual void ClearDxData();		// Libère les données Dx
	virtual bool IsDxReady() const;	// Vérifie si les données Dx sont prètes

protected:

	//-- Données protégées

	IDirect3DVertexDeclaration9	*m_pVertexDeclaration;
	IDirect3DVertexBuffer9		*m_pVertexBuffer;
	int		m_VBSize;

	//-- Méthodes protégées

	void CommonInit();	// Initialisation commune à tous les constructeurs

};

//******************************************************************
#endif		//_Quad_H

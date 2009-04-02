#ifndef		_Quad_H
#define		_Quad_H

//******************************************************************

#include	"Object2D.h"

//******************************************************************

class Quad : public Object2D
{
public:

	//-- M�thodes publiques
	
	Quad( int width, int height, const Color4f &color );
	Quad( const Vector2f &size, const Color4f &color );
	virtual ~Quad();

	virtual void Draw();		// Affiche l'objet

	//-- M�thodes sp�cifiques pour DirectX (utilis�es dans le Renderer)
	
	virtual void InitDxData();		// Initialise les donn�es Dx
	virtual void ClearDxData();		// Lib�re les donn�es Dx
	virtual bool IsDxReady() const;	// V�rifie si les donn�es Dx sont pr�tes

protected:

	//-- Donn�es prot�g�es

	IDirect3DVertexDeclaration9	*m_pVertexDeclaration;
	IDirect3DVertexBuffer9		*m_pVertexBuffer;
	int		m_VBSize;

	//-- M�thodes prot�g�es

	void CommonInit();	// Initialisation commune � tous les constructeurs

};

//******************************************************************
#endif		//_Quad_H

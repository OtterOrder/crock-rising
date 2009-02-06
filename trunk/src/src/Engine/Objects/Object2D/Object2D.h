#ifndef		_Object2D_H
#define		_Object2D_H

//******************************************************************

#include	<list>
#include	"Core/Math.h"

//******************************************************************

class Object2D
{
public:

	// Liste de r�f�rences sur les objets 2D
	static std::list< Object2D* > RefList;
	
	//-- M�thodes publiques
	
	Object2D();
	virtual ~Object2D();

	virtual void Draw() const =0;								// Affiche l'objet
	virtual void Update();										// Update, � surcharger

	void SetPosition( float posX, float posY );					// Change la position
	void SetPosition( const Point2f &position );				// Change la position
	Point2f GetPosition() const;								// Donne la position
	
	void SetScale( float scaleX, float scaleY );				// Change l'�chelle
	void SetScale( const Vector2f &scale );						// Change l'�chelle
	Vector2f GetScale() const;									// Donne l'�chelle

	void SetAlpha( float alpha );								// Change l'alpha (0->1)
	float GetAlpha() const;										// Donne l'alpha

	void SetVisibility( bool isVisible );						// Rend visible/invisible l'objet
	bool IsVisible() const;										// V�rifie si l'objet est visible

	// M�thodes sp�cifiques pour DirectX, appel�es lors
	// de la cr�ation/destruction/perte du device Dx.
	virtual void InitDxData(){}									// Initialise les donn�es Dx
	virtual void ClearDxData(){}								// Lib�re les donn�es Dx


protected:

	//-- Structures prot�g�es

	struct O2DVertex	// Vertex d�claration pour les objets 2D
	{
		Vector4f	position;	// xyzrhw
		Color4f		color;		// rgba
		Vector2f	texCoord;	// uv
	};
	
	//-- Donn�es prot�g�es
	
	Vector3f	m_Size;				// Taille
	Vector3f	m_Position;			// Position
	Vector3f	m_Scale;			// Echelle
	Color4f		m_Color;			// Couleur
	bool		m_IsVisible;		// Si l'objet est visible

	//-- M�thodes prot�g�es

	void WorldMatrix( D3DMATRIX *matrix ) const;	// Calcule la matrice de l'objet dans le rep�re world

};

//******************************************************************
#endif		//_Object2D_H

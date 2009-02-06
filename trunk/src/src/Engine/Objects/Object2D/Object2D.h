#ifndef		_Object2D_H
#define		_Object2D_H

//******************************************************************

#include	<list>
#include	"Core/Math.h"

//******************************************************************

class Object2D
{
public:

	// Liste de références sur les objets 2D
	static std::list< Object2D* > RefList;
	
	//-- Méthodes publiques
	
	Object2D();
	virtual ~Object2D();

	virtual void Draw() const =0;								// Affiche l'objet
	virtual void Update();										// Update, à surcharger

	void SetPosition( float posX, float posY );					// Change la position
	void SetPosition( const Point2f &position );				// Change la position
	Point2f GetPosition() const;								// Donne la position
	
	void SetScale( float scaleX, float scaleY );				// Change l'échelle
	void SetScale( const Vector2f &scale );						// Change l'échelle
	Vector2f GetScale() const;									// Donne l'échelle

	void SetAlpha( float alpha );								// Change l'alpha (0->1)
	float GetAlpha() const;										// Donne l'alpha

	void SetVisibility( bool isVisible );						// Rend visible/invisible l'objet
	bool IsVisible() const;										// Vérifie si l'objet est visible

	// Méthodes spécifiques pour DirectX, appelées lors
	// de la création/destruction/perte du device Dx.
	virtual void InitDxData(){}									// Initialise les données Dx
	virtual void ClearDxData(){}								// Libère les données Dx


protected:

	//-- Structures protégées

	struct O2DVertex	// Vertex déclaration pour les objets 2D
	{
		Vector4f	position;	// xyzrhw
		Color4f		color;		// rgba
		Vector2f	texCoord;	// uv
	};
	
	//-- Données protégées
	
	Vector3f	m_Size;				// Taille
	Vector3f	m_Position;			// Position
	Vector3f	m_Scale;			// Echelle
	Color4f		m_Color;			// Couleur
	bool		m_IsVisible;		// Si l'objet est visible

	//-- Méthodes protégées

	void WorldMatrix( D3DMATRIX *matrix ) const;	// Calcule la matrice de l'objet dans le repère world

};

//******************************************************************
#endif		//_Object2D_H

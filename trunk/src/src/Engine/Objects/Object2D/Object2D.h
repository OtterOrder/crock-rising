#ifndef		_Object2D_H
#define		_Object2D_H

//******************************************************************

#include	<list>
#include	<vector>
#include	"Core/Math.h"

//******************************************************************

enum O2DHotPoint
{
	O2D_HOTPOINT_V0,		// Vertex 0 (par défaut)
	//O2D_HOTPOINT_CENTER,	// Isobarycentre des vertices
	//O2D_HOTPOINT_INDEX		// Un vertex de l'objet
};

//******************************************************************

class Object2D
{
public:

	// Liste de références sur les objets 2D
	static std::list< Object2D* > RefList;
	
	//-- Méthodes publiques
	
	Object2D();
	virtual ~Object2D();

	virtual void Draw() =0;		// Affiche l'objet
	virtual void Update();		// Update, à surcharger

	// Position
	void SetPosition( float posX, float posY );
	void SetPosition( const Point2f &position );
	//void SetHotPoint( O2DHotPoint hotPoint, int vertex = 0 );
	Point2f GetPosition() const;
	
	// Echelle
	void SetScale( float scaleX, float scaleY );
	void SetScale( const Vector2f &scale );
	Vector2f GetScale() const;

	// Rotation
	void SetRotation( float angle );
	float GetRotation() const;

	// Couleur
	void SetColor( const Color4f &color );
	void SetAlpha( float alpha );
	Color4f GetColor() const;
	float GetAlpha() const;

	// Visibilité (optim)
	void Show();
	void Hide();
	bool IsHidden() const;

	//-- Méthodes spécifiques pour DirectX (utilisées dans le Renderer)
	
	virtual void InitDxData(){}		// Initialise les données Dx
	virtual void ClearDxData(){}	// Libère les données Dx

protected:

	//-- Structures protégées

	struct Vertex
	{
		Vector4f	position;	// POSITION (xyzrhw)
		Color4f		color;		// COLOR (rgba)
		Vector2f	texCoord;	// TEXCOORD (uv)

		// Génére la vertex déclaration correspondante..
		static void GenDeclaration( std::vector<D3DVERTEXELEMENT9> *vElements );
	};

	//-- Données protégées
	
	float			m_Width;			// Largeur
	float			m_Height;			// Hauteur
	Color4f			m_Color;			// Couleur
	O2DHotPoint		m_HotPoint;			// Point chaud
	int				m_HotPointIndex;	// Index du point chaud (O2D_HOTPOINT_INDEX)
	bool			m_IsHidden;			// Si l'objet est caché
	bool			m_IsDxReady;		// Si les données Dx sont initialisées

	Vector3f		m_Position;			// Position (du point chaud)
	Vector3f		m_Scale;			// Echelle
	Vector3f		m_Rotation;			// Rotation

	//-- Méthodes protégées

	void WorldMatrix( D3DMATRIX *matrix ) const;	// Calcule la matrice de l'objet dans le repère world
	void GenQuad( Vertex *vertices ) const;			// Génère les points d'un quad à partir des données de l'objet

};

//******************************************************************
#endif		//_Object2D_H

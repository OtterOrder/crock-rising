#ifndef		_Object2D_H
#define		_Object2D_H

//******************************************************************

#include	<list>
#include	<vector>
#include	"Core/Math.h"

//******************************************************************

enum O2DHotPoint
{
	O2D_HOTPOINT_V0,		// Vertex 0 (par d�faut)
	//O2D_HOTPOINT_CENTER,	// Isobarycentre des vertices
	//O2D_HOTPOINT_INDEX		// Un vertex de l'objet
};

//******************************************************************

class Object2D
{
public:

	// Liste de r�f�rences sur les objets 2D
	static std::list< Object2D* > RefList;
	
	//-- M�thodes publiques
	
	Object2D();
	virtual ~Object2D();

	virtual void Draw() =0;		// Affiche l'objet
	virtual void Update();		// Update, � surcharger

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

	// Visibilit� (optim)
	void Show();
	void Hide();
	bool IsHidden() const;

	//-- M�thodes sp�cifiques pour DirectX (utilis�es dans le Renderer)
	
	virtual void InitDxData(){}		// Initialise les donn�es Dx
	virtual void ClearDxData(){}	// Lib�re les donn�es Dx

protected:

	//-- Structures prot�g�es

	struct Vertex
	{
		Vector4f	position;	// POSITION (xyzrhw)
		Color4f		color;		// COLOR (rgba)
		Vector2f	texCoord;	// TEXCOORD (uv)

		// G�n�re la vertex d�claration correspondante..
		static void GenDeclaration( std::vector<D3DVERTEXELEMENT9> *vElements );
	};

	//-- Donn�es prot�g�es
	
	float			m_Width;			// Largeur
	float			m_Height;			// Hauteur
	Color4f			m_Color;			// Couleur
	O2DHotPoint		m_HotPoint;			// Point chaud
	int				m_HotPointIndex;	// Index du point chaud (O2D_HOTPOINT_INDEX)
	bool			m_IsHidden;			// Si l'objet est cach�
	bool			m_IsDxReady;		// Si les donn�es Dx sont initialis�es

	Vector3f		m_Position;			// Position (du point chaud)
	Vector3f		m_Scale;			// Echelle
	Vector3f		m_Rotation;			// Rotation

	//-- M�thodes prot�g�es

	void WorldMatrix( D3DMATRIX *matrix ) const;	// Calcule la matrice de l'objet dans le rep�re world
	void GenQuad( Vertex *vertices ) const;			// G�n�re les points d'un quad � partir des donn�es de l'objet

};

//******************************************************************
#endif		//_Object2D_H

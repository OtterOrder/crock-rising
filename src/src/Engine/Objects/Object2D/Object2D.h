#ifndef		_Object2D_H
#define		_Object2D_H

//******************************************************************

#include	<list>
#include	<vector>
#include	"Core/Math.h"

//******************************************************************

class Object2D
{
public:

	// Liste de r�f�rences sur les objets 2D
	static std::list< Object2D* > RefList;

	//-- Enum publiques

	enum HotPoint
	{
		HOTPOINT_V0,		// Vertex 0 (par d�faut)
		//HOTPOINT_CENTER,	// Isobarycentre des vertices
		//HOTPOINT_INDEX		// Un vertex de l'objet
	};
	
	//-- M�thodes publiques
	
	Object2D();
	virtual ~Object2D();

	virtual void Draw() =0;		// Affiche l'objet
	virtual void Update();		// Update, � surcharger

	// Taille
	void SetSize( int width, int height );
	void SetSize( const Vector2f &size );
	int GetWidth() const;
	int GetHeight() const;

	// Couleur
	void SetColor( const Color4f &color );
	void SetAlpha( float alpha );
	Color4f GetColor() const;
	float GetAlpha() const;

	// Priorit� (ordre d'affichage)
	void SetPriority( int priority );	// 0>>255
	int GetPriority() const;

	// Visibilit� (optim)
	void Show();
	void Hide();
	bool IsHidden() const;

	// Position
	void SetPosition( float posX, float posY );
	void SetPosition( const Point2f &position );
	//void SetHotPoint( HotPoint hotPoint, int vertex = 0 );
	Point2f GetPosition() const;
	
	// Echelle
	void SetScale( float scaleX, float scaleY );
	void SetScale( const Vector2f &scale );
	Vector2f GetScale() const;

	// Rotation
	void SetRotation( float angle );
	float GetRotation() const;

	//-- M�thodes sp�cifiques pour DirectX (utilis�es dans le Renderer)
	
	virtual void InitDxData(){}			// Initialise les donn�es Dx
	virtual void ClearDxData(){}		// Lib�re les donn�es Dx
	virtual bool IsDxReady() const		// V�rifie si les donn�es Dx sont pr�tes
	{
		return false;
	}

	// Compare les priorit�s des objets
	static bool	ComparePriority( const Object2D *o1, const Object2D *o2 )
	{
		// o1 est avant o2 si sa priorit� est plus faible, la
		// priorit� la plus faible �tant 255, la plus forte 0.
		return o1->m_Priority > o2->m_Priority;
	}

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
	
	int				m_Width;			// Largeur
	int				m_Height;			// Hauteur
	Color4f			m_Color;			// Couleur
	HotPoint		m_HotPoint;			// Point chaud
	int				m_HotPointIndex;	// Index du point chaud (HOTPOINT_INDEX)
	int				m_Priority;			// Priorit� d'affichage de l'objet (0>>255)
	bool			m_IsHidden;			// Si l'objet est cach�

	Vector3f		m_Position;			// Position (du point chaud)
	Vector3f		m_Scale;			// Echelle
	Vector3f		m_Rotation;			// Rotation

	//-- M�thodes prot�g�es

	void WorldMatrix( D3DMATRIX *matrix ) const;	// Calcule la matrice de l'objet dans le rep�re world
	void GenQuad( Vertex *vertices ) const;			// G�n�re les points d'un quad � partir des donn�es de l'objet

};

//******************************************************************
#endif		//_Object2D_H

#ifndef		_SCENEOBJECT2D_H_
#define		_SCENEOBJECT2D_H_

//******************************************************************

#include	<vector>
#include	"Object2D.h"

//******************************************************************

class SceneObject2D : public Object2D
{
public:

	// Liste de r�f�rences sur les scene objets 2D
	static std::list< SceneObject2D* > RefList;

	//-- M�thodes publiques

	SceneObject2D();
	virtual ~SceneObject2D();

	// Affichage. A surcharger
	virtual void Draw() =0;

	// Point chaud (relatif � v0)
	void SetHotPoint( s32 posX, s32 posY );
	void SetHotPoint( const Vector2i &hotPoint );
	Vector2i GetHotPoint() const;

	// Couleur
	void SetColor( const Color4f &color );
	void SetAlpha( float alpha );
	Color4f GetColor() const;
	float GetAlpha() const;

	// Priorit� (ordre d'affichage)
	void SetPriority( u8 priority );
	u8 GetPriority() const;

	// Visibilit� (optim)
	void Show();
	void Hide();
	inline bool IsHidden() const { return m_IsHidden; }

	// Compare les priorit�s, utile pour trier la liste
	static bool	ComparePriority( const SceneObject2D *pObj1, const SceneObject2D *pObj2 );

	// Gestion des donn�es DirectX
	virtual void InitDxData();		// Initialise les donn�es Dx
	virtual void ClearDxData();		// Lib�re les donn�es Dx
	virtual bool IsDxReady() const;	// V�rifie si les donn�es Dx sont pr�tes

protected:

	// Vertex pour les objets 2D
	struct Vertex
	{
		Vector4f	position;	// POSITIONT (xyzrhw)
		Vector2f	texCoord;	// TEXCOORD (uv)
		Color4f		color;		// COLOR (rgba)

		// G�n�re la vertex d�claration correspondante
		static void GenDeclaration( std::vector<D3DVERTEXELEMENT9> *vElements );
	};

	//-- Donn�es prot�g�es

	s32				m_NbVertices;		// Nombre de sommets
	Vertex			*m_Vertices;		// Sommets de l'objet
	Vector2i		m_HotPoint;			// Point chaud relatif � m_v[0]
	Color4f			m_Color;			// Couleur de l'objet
	u8				m_Priority;			// Priorit� d'affichage de l'objet (0>>255)
	bool			m_IsHidden;			// Si l'objet est cach�

	// Donn�es pour l'affichage DirectX
	LPDIRECT3DVERTEXDECLARATION9	m_VertexDeclaration;	// Vertex d�claration
	LPDIRECT3DVERTEXBUFFER9			m_VertexBuffer;			// Vertex buffer
	s32								m_VertexBufferSize;		// Taille du vertex buffer

	//-- M�thodes prot�g�es

	void CopyVerticesToVertexBuffer();	// Copie les sommets de l'objet dans le vertex buffer
	virtual void dirty_Refresh() {}		// Met � jour tout le bazar. A surcharger

};

//******************************************************************
#endif		//_SCENEOBJECT2D_H_
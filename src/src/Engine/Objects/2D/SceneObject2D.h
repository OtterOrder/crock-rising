#ifndef		_SCENEOBJECT2D_H_
#define		_SCENEOBJECT2D_H_

//******************************************************************

#include	<vector>
#include	"Object2D.h"

//******************************************************************

class SceneObject2D : public Object2D
{
public:

	// Liste de références sur les scene objets 2D
	static std::list< SceneObject2D* > RefList;

	//-- Méthodes publiques

	SceneObject2D();
	virtual ~SceneObject2D();

	// Affichage. A surcharger
	virtual void Draw() =0;

	// Point chaud (relatif à v0)
	void SetHotPoint( s32 posX, s32 posY );
	void SetHotPoint( const Vector2i &hotPoint );
	Vector2i GetHotPoint() const;

	// Couleur
	void SetColor( const Color4f &color );
	void SetAlpha( float alpha );
	Color4f GetColor() const;
	float GetAlpha() const;

	// Priorité (ordre d'affichage)
	void SetPriority( u8 priority );
	u8 GetPriority() const;

	// Visibilité (optim)
	void Show();
	void Hide();
	inline bool IsHidden() const { return m_IsHidden; }

	// Compare les priorités, utile pour trier la liste
	static bool	ComparePriority( const SceneObject2D *pObj1, const SceneObject2D *pObj2 );

	// Gestion des données DirectX
	virtual void InitDxData();		// Initialise les données Dx
	virtual void ClearDxData();		// Libère les données Dx
	virtual bool IsDxReady() const;	// Vérifie si les données Dx sont prètes

protected:

	// Vertex pour les objets 2D
	struct Vertex
	{
		Vector4f	position;	// POSITIONT (xyzrhw)
		Vector2f	texCoord;	// TEXCOORD (uv)
		Color4f		color;		// COLOR (rgba)

		// Génére la vertex déclaration correspondante
		static void GenDeclaration( std::vector<D3DVERTEXELEMENT9> *vElements );
	};

	//-- Données protégées

	s32				m_NbVertices;		// Nombre de sommets
	Vertex			*m_Vertices;		// Sommets de l'objet
	Vector2i		m_HotPoint;			// Point chaud relatif à m_v[0]
	Color4f			m_Color;			// Couleur de l'objet
	u8				m_Priority;			// Priorité d'affichage de l'objet (0>>255)
	bool			m_IsHidden;			// Si l'objet est caché

	// Données pour l'affichage DirectX
	LPDIRECT3DVERTEXDECLARATION9	m_VertexDeclaration;	// Vertex déclaration
	LPDIRECT3DVERTEXBUFFER9			m_VertexBuffer;			// Vertex buffer
	s32								m_VertexBufferSize;		// Taille du vertex buffer

	//-- Méthodes protégées

	void CopyVerticesToVertexBuffer();	// Copie les sommets de l'objet dans le vertex buffer
	virtual void dirty_Refresh() {}		// Met à jour tout le bazar. A surcharger

};

//******************************************************************
#endif		//_SCENEOBJECT2D_H_
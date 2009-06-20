#ifndef		_SCENEOBJECT2D_H_
#define		_SCENEOBJECT2D_H_

//******************************************************************

#include	<vector>
#include	"Object2D.h"

//******************************************************************

class Shader;

class SceneObject2D : public Object2D
{
public:

	// Liste de références sur les scene objets 2D
	static std::list< SceneObject2D* > RefList;

	//-- Méthodes publiques

	SceneObject2D();
	virtual ~SceneObject2D();

	// Initialisation
	void Init();
	
	// Affichage. A surcharger
	virtual void Draw() =0;

	// Point chaud (relatif à v0)
	void SetHotPoint( s32 posX, s32 posY );
	void SetHotPoint( const Vector2i &hotPoint );
	Vector2i GetHotPoint() const;

	// Couleur
	void SetColor( const Color4f &color );
	Color4f GetColor() const;
	void SetAlpha( float alpha );
	float GetAlpha() const;

	// Priorité (ordre d'affichage)
	void SetPriority( u8 priority );
	u8 GetPriority() const;

	// Visibilité (optim)
	inline bool IsHidden() const { return m_Color.a <= 0.f; }

	// Shader
	void SetShader( const std::string &shaderName );
	const std::string& GetShaderName() const;
	Shader* const GetShader() const;

	// Collision. A surcharger
	virtual bool IsCollide( s32 posX, s32 posY ) { return false; }

	// Compare les priorités, utile pour trier la liste
	static bool	ComparePriority( const SceneObject2D *pObj1, const SceneObject2D *pObj2 );

	// Gestion des entités DirectX
	virtual void OnCreateDevice();
	virtual void OnResetDevice();
	virtual void OnLostDevice();
	virtual void OnDestroyDevice();
	virtual bool IsDrawable() const;

protected:

	// Vertex pour les objets 2D
	struct Vertex
	{
		Vector4f	position;	// POSITIONT (xyzrhw)
		Vector2f	texCoord;	// TEXCOORD (uv)

		// Génére la vertex déclaration correspondante
		static void GenDeclaration( std::vector<D3DVERTEXELEMENT9> *vElements );
	};

	//-- Données protégées

	s32				m_NbVertices;		// Nombre de sommets
	Vertex			*m_Vertices;		// Sommets de l'objet
	Vector2i		m_HotPoint;			// Point chaud relatif à m_v[0]
	Color4f			m_Color;			// Couleur de l'objet
	u8				m_Priority;			// Priorité d'affichage de l'objet (0>>255)

	std::string		m_ShaderName;		// Nom du shader
	Shader			*m_Shader;			// Shader de l'objet

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
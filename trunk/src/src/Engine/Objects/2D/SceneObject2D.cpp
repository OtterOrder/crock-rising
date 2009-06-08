#include	"SceneObject2D.h"

#include	<assert.h>
#include	"Renderer/Renderer.h"
#include	"Resources/ResourceManager.h"
#include	"Resources/Shader.h"

//******************************************************************

#define		O2D_DEFAULT_HOT_POINT		(Vector2i( 0, 0 ))
#define		O2D_DEFAULT_COLOR			(Color4f( 1.f, 1.f, 1.f, 1.f ))
#define		O2D_DEFAULT_OPACITY			(1.f)
#define		O2D_DEFAULT_PRIORITY		(0)

//******************************************************************

std::list< SceneObject2D* > SceneObject2D::RefList;

//******************************************************************

//**********************************************************
// Constructeur par défaut.
//**********************************************************
SceneObject2D::SceneObject2D()
: Object2D()
{
	m_NbVertices	= 0;
	m_Vertices		= NULL;
	m_HotPoint		= O2D_DEFAULT_HOT_POINT;
	m_Color			= O2D_DEFAULT_COLOR;
	m_Opacity		= O2D_DEFAULT_OPACITY;
	m_Priority		= O2D_DEFAULT_PRIORITY;

	m_ShaderName	= "default2d.fx";
	m_Shader		= NULL;

	m_VertexDeclaration	= NULL;
	m_VertexBuffer		= NULL;
	m_VertexBufferSize	= 0;

	SceneObject2D::RefList.push_front( this );
}

//**********************************************************
// Destructeur.
//**********************************************************
SceneObject2D::~SceneObject2D()
{
	SceneObject2D::RefList.remove( this );
	ClearDxData();
}

//**********************************************************
// Change le point chaud. Il s'exprime en coordonnées
// relatives à v0 (sommet 0 de l'objet).
//
// Note: si le point chaud est (0,0), il est égal à v0.
//
// @param[in]	posX : Coordonnée x (pixels)
// @param[in]	posY : Coordonnée y (pixels)
//**********************************************************
void SceneObject2D::SetHotPoint( s32 posX, s32 posY )
{
	if( posX != m_HotPoint.x || posY != m_HotPoint.y )
	{
		m_HotPoint.x = posX;
		m_HotPoint.y = posY;
		activate_dirty();
	}
}

//**********************************************************
// Change le point chaud.
// @param[in]	hotPoint : Point chaud
//**********************************************************
void SceneObject2D::SetHotPoint( const Vector2i &hotPoint )
{
	SetHotPoint( hotPoint.x, hotPoint.y );
}

//**********************************************************
// Donne le point chaud. Attention, les coordonnées sont
// relatives à v0.

// Note: les coordonnées absolues du point chaud sont
// données par GetPosition.

// @return	Le point chaud
//**********************************************************
Vector2i SceneObject2D::GetHotPoint() const
{
	return m_HotPoint;
}

//**********************************************************
// Change la couleur.
// @param[in]	color : RGBA (0->1)
//**********************************************************
void SceneObject2D::SetColor( const Color4f &color )
{
	m_Color = color;
	activate_dirty();
}

//**********************************************************
// Donne la couleur.
// @return	La couleur RGBA
//**********************************************************
Color4f SceneObject2D::GetColor() const
{
	return m_Color;
}

//**********************************************************
// Change l'opacité (= transparence).
// @param[in]	opacity : Opacité (0->1)
//**********************************************************
void SceneObject2D::SetOpacity( float opacity )
{
	m_Opacity = opacity;
	activate_dirty();
}

//**********************************************************
// Donne l'opacité.
// @return	L'opacité (woohoo)
//**********************************************************
float SceneObject2D::GetOpacity() const
{
	return m_Opacity;
}

//**********************************************************
// Change la priorité (ordre d'affichage).
// @param[in]	priority : Priorité entre 0 et 255
//				0 : devant, 255 : derrière
//**********************************************************
void SceneObject2D::SetPriority( u8 priority )
{
	m_Priority = priority;
}

//**********************************************************
// Donne la priorité.
// @return	La priorité
//**********************************************************
u8 SceneObject2D::GetPriority() const
{
	return m_Priority;
}

//**********************************************************
// Change le shader appliqué à l'objet.
// @param[in]	shaderName : Nom de la ressource shader
//**********************************************************
void SceneObject2D::SetShader( const std::string &shaderName )
{
	if( m_Shader && shaderName != m_ShaderName )
	{
		ResourceManager::GetInstance()->Remove<Shader>( m_ShaderName );
		m_Shader = ResourceManager::GetInstance()->Load<Shader>( shaderName );
	}
	m_ShaderName = shaderName;
}

//**********************************************************
// Donne le shader courant de l'objet.
// @return	Nom de la ressource shader
//**********************************************************
const std::string& SceneObject2D::GetShader() const
{
	return m_ShaderName;
}

//**********************************************************
// Compare les priorités des objets. Permet de trier les
// objets par ordre de priorité croissante (pour l'affichage).
//**********************************************************
bool SceneObject2D::ComparePriority( const SceneObject2D *pObj1, const SceneObject2D *pObj2 )
{
	// pObj1 est avant pObj2 si sa priorité est plus faible, la
	// priorité la plus faible étant 255, la plus forte 0.
	return pObj1->m_Priority > pObj2->m_Priority;
}

//**********************************************************
// Initialise les données Dx.
//**********************************************************
void SceneObject2D::InitDxData()
{
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetInstance()->m_pd3dDevice;
	std::vector<D3DVERTEXELEMENT9> elements;
	
	m_VertexBufferSize = m_NbVertices*sizeof(Vertex);
	
	// Vertex buffer
	if( !m_VertexBuffer )
	{
		pDevice->CreateVertexBuffer(
			m_VertexBufferSize,
			NULL,
			0,
			D3DPOOL_DEFAULT,
			&m_VertexBuffer,
			NULL
		);
	}

	// Vertex déclaration
	if( !m_VertexDeclaration )
	{
		Vertex::GenDeclaration( &elements );
		pDevice->CreateVertexDeclaration(
			&elements[0],
			&m_VertexDeclaration
		);
	}

	// Shader
	if( !m_Shader )
	{
		m_Shader = ResourceManager::GetInstance()->Load<Shader>( m_ShaderName );
	}
}

//**********************************************************
// Libère les données Dx.
//**********************************************************
void SceneObject2D::ClearDxData()
{
	if( m_VertexBuffer )
	{
		m_VertexBuffer->Release();
		m_VertexBuffer = NULL;
	}
	if( m_VertexDeclaration )
	{
		m_VertexDeclaration->Release();
		m_VertexDeclaration = NULL;
	}
	if( m_Shader )
	{
		ResourceManager::GetInstance()->Remove<Shader>( m_ShaderName );
		m_Shader = NULL;
	}
}

//**********************************************************
// Vérifie si les données Dx sont prètes.
//**********************************************************
bool SceneObject2D::IsDxReady() const
{
	return m_VertexDeclaration
		&& m_VertexBuffer;
}

//**********************************************************
// Copie les sommets de l'objet dans le vertex buffer.
// On peut appeler cette méthode à la fin de dirty_Refresh
// par exemple..
//**********************************************************
void SceneObject2D::CopyVerticesToVertexBuffer()
{
	assert( m_NbVertices > 0 );
	if( IsDxReady() )
	{
		void *pData;
		m_VertexBuffer->Lock( 0, m_VertexBufferSize, (void**)&pData, 0 );
		memcpy( pData, m_Vertices, m_VertexBufferSize );
		m_VertexBuffer->Unlock();
	}
}

//**********************************************************
// Génére la vertex declaration correspondante à la
// structure du vertex.
// @param[out]	vElements : tableau de D3DVERTEXELEMENT9
//**********************************************************
void SceneObject2D::Vertex::GenDeclaration( std::vector<D3DVERTEXELEMENT9> *vElements )
{
	D3DVERTEXELEMENT9 element;
	int offset = 0;

	// Propriétés communes
	element.Stream		= 0;
	element.Method		= D3DDECLMETHOD_DEFAULT;
	element.UsageIndex	= 0;

	// POSITION (xyzrhw)
	element.Offset	= offset;
	element.Type	= D3DDECLTYPE_FLOAT4;
	element.Usage	= D3DDECLUSAGE_POSITIONT;
	offset			+= sizeof(Vector4f);
	vElements		->push_back( element );

	// TEXCOORD (uv)
	element.Offset	= offset;
	element.Type	= D3DDECLTYPE_FLOAT2;
	element.Usage	= D3DDECLUSAGE_TEXCOORD;
	offset			+= sizeof(Vector2f);
	vElements		->push_back( element );

	// COLOR (rgba)
	element.Offset	= offset;
	element.Type	= D3DDECLTYPE_FLOAT4;
	element.Usage	= D3DDECLUSAGE_COLOR;
	offset			+= sizeof(Color4f);
	vElements		->push_back( element );

	// Element de fin
	D3DVERTEXELEMENT9 endElement = D3DDECL_END();
	vElements->push_back( endElement );
}

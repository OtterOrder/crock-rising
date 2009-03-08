#include	"Quad.h"

#include	"Renderer/Renderer.h"

using namespace std;

//******************************************************************

//**********************************************************
// Constructeur.
// @param[in]	width : largeur
// @param[in]	height : hauteur
// @param[in]	color : couleur
//**********************************************************
Quad::Quad( float width, float height, const Color4f &color )
: Object2D()
{
	m_Width		= width;
	m_Height	= height;
	m_Color		= color;
}

//**********************************************************
// Constructeur.
// @param[in]	size : taille
// @param[in]	color : couleur
//**********************************************************
Quad::Quad( const Vector2f &size, const Color4f &color )
: Object2D()
{
	m_Width		= size.x;
	m_Height	= size.y;
	m_Color		= color;
}

//**********************************************************
// Destructeur.
//**********************************************************
Quad::~Quad()
{
	ClearDxData();
}

//**********************************************************
// Affiche l'objet.
//**********************************************************
void Quad::Draw()
{
	if( !m_IsDxReady )
	{
		// Si les données directx ne sont pas initialisées,
		// on le fait au premier affichage, ça évite de le
		// faire dans les constructeurs..
		InitDxData();
	}
	
	Vertex vertices[4];
	LPDIRECT3DDEVICE9 pDevice;
	void *pData;

	pDevice = Renderer::GetInstance()->m_pd3dDevice;
	
	// On génère les points du quad
	GenQuad( vertices );
	
	// Remplissage du vertex buffer
	m_pVertexBuffer->Lock( 0, m_VBSize, (void**)&pData, 0 );
	memcpy( pData, vertices, m_VBSize );
	m_pVertexBuffer->Unlock();
	
	// Rendu
	pDevice->SetVertexDeclaration( m_pVertexDeclaration );
	pDevice->SetStreamSource( 0, m_pVertexBuffer, 0, sizeof(Vertex) );
	pDevice->DrawPrimitive( D3DPT_TRIANGLEFAN, 0, 2 );
}

//**********************************************************
// Initialise les données Dx.
//**********************************************************
void Quad::InitDxData()
{
	if( m_IsDxReady )
		return;

	LPDIRECT3DDEVICE9			pDevice;
	vector<D3DVERTEXELEMENT9>	elements;

	pDevice		= Renderer::GetInstance()->m_pd3dDevice;
	m_VBSize	= 4*sizeof(Vertex);

	// Vertex déclaration
	Vertex::GenDeclaration( &elements );
	pDevice->CreateVertexDeclaration(
		&elements[0],
		&m_pVertexDeclaration
	);

	// Vertex buffer
	pDevice->CreateVertexBuffer(
		m_VBSize,
		NULL,
		0,
		D3DPOOL_DEFAULT,
		&m_pVertexBuffer,
		NULL
	);

	m_IsDxReady = true;
}

//**********************************************************
// Libère les données Dx.
//**********************************************************
void Quad::ClearDxData()
{
	if( m_IsDxReady )
	{
		m_pVertexDeclaration->Release();
		m_pVertexBuffer->Release();
	}
	m_IsDxReady = false;
}

#include	"Quad.h"

//#include	<assert.h>
#include	"Renderer/Renderer.h"

using namespace std;

//******************************************************************

//**********************************************************
// Initialisation commune à tous les constructeurs.
//**********************************************************
void Quad::CommonInit()
{
	m_pVertexDeclaration	= NULL;
	m_pVertexBuffer			= NULL;
}

//**********************************************************
// Constructeur.
// @param[in]	width : largeur
// @param[in]	height : hauteur
// @param[in]	color : couleur
//**********************************************************
Quad::Quad( int width, int height, const Color4f &color )
: Object2D()
{
	m_Width		= width;
	m_Height	= height;
	m_Color		= color;

	CommonInit();
}

//**********************************************************
// Constructeur.
// @param[in]	size : { largeur, hauteur }
// @param[in]	color : couleur
//**********************************************************
Quad::Quad( const Vector2f &size, const Color4f &color )
: Object2D()
{
	m_Width		= (int)size.x;
	m_Height	= (int)size.y;
	m_Color		= color;
	
	CommonInit();
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
	if( !IsDxReady() )
	{
		// Si les données directx ne sont pas initialisées,
		// on le fait au premier affichage, ça évite de le
		// faire dans les constructeurs..
		InitDxData();
	}
	
	Vertex vertices[O2D_VERTICES];
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
	LPDIRECT3DDEVICE9			pDevice;
	vector<D3DVERTEXELEMENT9>	elements;

	pDevice		= Renderer::GetInstance()->m_pd3dDevice;
	m_VBSize	= O2D_VERTICES*sizeof(Vertex);

	// Vertex déclaration
	if( !m_pVertexDeclaration )
	{
		Vertex::GenDeclaration( &elements );
		pDevice->CreateVertexDeclaration(
			&elements[0],
			&m_pVertexDeclaration
		);
	}

	// Vertex buffer
	if( !m_pVertexBuffer )
	{
		pDevice->CreateVertexBuffer(
			m_VBSize,
			NULL,
			0,
			D3DPOOL_DEFAULT,
			&m_pVertexBuffer,
			NULL
		);
	}
}

//**********************************************************
// Libère les données Dx.
//**********************************************************
void Quad::ClearDxData()
{
	if( m_pVertexDeclaration )
	{
		m_pVertexDeclaration->Release();
		m_pVertexDeclaration = NULL;
	}
	
	if( m_pVertexBuffer )
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = NULL;
	}
}

//**********************************************************
// Vérifie si les données Dx sont prètes.
//**********************************************************
bool Quad::IsDxReady() const
{
	return m_pVertexDeclaration
		&& m_pVertexBuffer;
}

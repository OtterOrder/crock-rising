#include	"RenderTarget.h"

#include	<assert.h>

//----------------------------------------------------------------------------------------------
RenderTarget::RenderTarget(void)
{
	m_pSurface = NULL;
	m_pTexture = NULL;
}

RenderTarget::RenderTarget(Coord _Size)
{
	m_pSurface = NULL;
	m_pTexture = NULL;

	m_Size = _Size;
}

//----------------------------------------------------------------------------------------------
RenderTarget::~RenderTarget(void)
{
	Release();
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void RenderTarget::Release ()
{
	if (m_pSurface)
	{
		m_pSurface->Release();
		m_pSurface = NULL;
	}

	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//----------------------------------------------------------------------------------------------
HRESULT RenderTarget::Create (LPDIRECT3DDEVICE9 _pDevice)
{
	assert (_pDevice);

	Release ();

	HRESULT result = D3DXCreateTexture(	_pDevice,
										m_Size.x,
										m_Size.y,
										1,
										D3DUSAGE_RENDERTARGET,
										D3DFMT_A8R8G8B8,
										D3DPOOL_DEFAULT,
										&m_pTexture );

	assert(m_pTexture);

	m_pTexture->GetSurfaceLevel(0, &m_pSurface);

	return result;
}

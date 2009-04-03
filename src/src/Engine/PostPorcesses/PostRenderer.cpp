#include	"PostRenderer.h"

#include	"Renderer/Renderer.h"
#include	<assert.h>

//----------------------------------------------------------------------------------------------
PostRenderer::PostRenderer(void)
{
	m_pBackBuffer		  = NULL;

	m_pSceneRenderTarget	= NULL;

	/*MotionBlur* pMotionBlur = new MotionBlur();
	m_pPostEffects.push_back(pMotionBlur);*/
}

//----------------------------------------------------------------------------------------------
PostRenderer::~PostRenderer(void)
{
	ReleaseSceneRender ();
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void PostRenderer::SetBackBuffer (LPDIRECT3DSURFACE9 _pBackBuffer)
{
	assert(_pBackBuffer);
	m_pBackBuffer = _pBackBuffer;
}

//----------------------------------------------------------------------------------------------
void PostRenderer::RenderPostEffects ()
{
	RECT windowRect;
	windowRect.left		= 0;
	windowRect.top		= 0;
	windowRect.right	= Renderer::GetInstance()->GetWindowWidth();
	windowRect.bottom	= Renderer::GetInstance()->GetWindowHeight();
	Renderer::GetInstance()->m_pd3dDevice->StretchRect(m_pSceneRenderTarget->GetSurface(), NULL, m_pBackBuffer, &windowRect, D3DTEXF_NONE);
}

/*/----------------------------------------------------------------------------------------------
void PostRenderer::SetSceneRenderTexture (LPDIRECT3DTEXTURE9 _pSceneRenderTexture)
{
	assert(_pSceneRenderTexture);
	m_pSceneRenderTexture = _pSceneRenderTexture;

	m_pSceneRenderTexture->GetSurfaceLevel(0, &m_pSceneRenderSurface);
}
//*/
//----------------------------------------------------------------------------------------------
void PostRenderer::ReleaseSceneRender ()
{
	if (m_pSceneRenderTarget)
	{
		delete m_pSceneRenderTarget;
		m_pSceneRenderTarget = NULL;
	}
}

//----------------------------------------------------------------------------------------------
HRESULT PostRenderer::CreateSceneRender (LPDIRECT3DDEVICE9 _pDevice, u32 _width, u32 _height)
{
	assert (_pDevice);

	ReleaseSceneRender ();
	
	m_pSceneRenderTarget = new RenderTarget(Coord(_width, _height));
	return m_pSceneRenderTarget->Create(_pDevice);
}

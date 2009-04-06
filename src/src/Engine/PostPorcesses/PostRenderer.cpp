#include	"PostRenderer.h"

#include	"Renderer/Renderer.h"
#include	<assert.h>

//----------------------------------------------------------------------------------------------
PostRenderer::PostRenderer(void)
{
	m_pBackBuffer				= NULL;

	m_pSceneRenderTarget		= NULL;
	m_pSceneRenderTargetTemp	= NULL;
/*
	MotionBlur* pMotionBlur = new MotionBlur(Renderer::GetInstance()->GetWindowWidth(), Renderer::GetInstance()->GetWindowHeight());
	m_pPostEffects.push_back(pMotionBlur);
//*/
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
/*
	for (u32 postEffect = 0; postEffect < m_pPostEffects.size(); postEffect++)
	{
		if (m_pPostEffects[postEffect])
			m_pPostEffects[postEffect]->Apply(&SceneObject::RefList);
	}
//*/
	if (! (m_pSceneRenderTarget && m_pBackBuffer))
		return;

	RECT windowRect;
	windowRect.left		= 0;
	windowRect.top		= 0;
	windowRect.right	= Renderer::GetInstance()->GetWindowWidth();
	windowRect.bottom	= Renderer::GetInstance()->GetWindowHeight();
	Renderer::GetInstance()->m_pd3dDevice->StretchRect(m_pSceneRenderTarget->GetSurface(), NULL, m_pBackBuffer, &windowRect, D3DTEXF_NONE);

	//// Delete Me !!!!
	//Sleep(10);
}

//----------------------------------------------------------------------------------------------
void PostRenderer::ReleaseSceneRender ()
{
	if (m_pSceneRenderTarget)
	{
		delete m_pSceneRenderTarget;
		m_pSceneRenderTarget = NULL;
	}
	if (m_pSceneRenderTargetTemp)
	{
		delete m_pSceneRenderTargetTemp;
		m_pSceneRenderTargetTemp = NULL;
	}
}

//----------------------------------------------------------------------------------------------
HRESULT PostRenderer::CreateSceneRender (LPDIRECT3DDEVICE9 _pDevice, u32 _width, u32 _height)
{
	//// ToDo : Change return value
	assert (_pDevice);

	ReleaseSceneRender ();
	
	//HRESULT renderTargetCrateation;

	m_pSceneRenderTarget		= new RenderTarget(_width, _height);
	m_pSceneRenderTarget->Create(_pDevice);

	m_pSceneRenderTargetTemp	= new RenderTarget(_width, _height);
	m_pSceneRenderTargetTemp->Create(_pDevice);

	return S_OK;
}

//----------------------------------------------------------------------------------------------
void PostRenderer::SwapSceneRender ()
{
	RenderTarget* _tempRT		= m_pSceneRenderTarget;
	m_pSceneRenderTarget		= m_pSceneRenderTargetTemp;
	m_pSceneRenderTargetTemp	= _tempRT;
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void PostRenderer::SetRenderTarget (LPDIRECT3DSURFACE9 _pRenderTarget)
{
	if (!_pRenderTarget)
		return;

	Renderer::GetInstance()->m_pd3dDevice->SetRenderTarget(0, _pRenderTarget);
}

//----------------------------------------------------------------------------------------------
LPDIRECT3DSURFACE9 PostRenderer::GetRenderTarget ()
{
	LPDIRECT3DSURFACE9 pRenderTarget = NULL;
	Renderer::GetInstance()->m_pd3dDevice->GetRenderTarget(0, &pRenderTarget);
	return pRenderTarget;
}

//----------------------------------------------------------------------------------------------
void PostRenderer::SetRenderTarget (u32 _level, LPDIRECT3DSURFACE9 _pRenderTarget)
{
	if (!_pRenderTarget)
		return;

	Renderer::GetInstance()->m_pd3dDevice->SetRenderTarget(_level, _pRenderTarget);
}

//----------------------------------------------------------------------------------------------
LPDIRECT3DSURFACE9 PostRenderer::GetRenderTarget (u32 _level)
{
	LPDIRECT3DSURFACE9 pRenderTarget = NULL;
	Renderer::GetInstance()->m_pd3dDevice->GetRenderTarget(_level, &pRenderTarget);
	return pRenderTarget;
}
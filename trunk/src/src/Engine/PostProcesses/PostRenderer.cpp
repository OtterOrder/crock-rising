#include	"PostRenderer.h"

#include	"Renderer/Renderer.h"
#include	<assert.h>

//----------------------------------------------------------------------------------------------
PostRenderer::PostRenderer(void)
{
	m_pBackBuffer				= NULL;

	m_pSceneRenderTarget		= NULL;
	m_pSceneRenderTargetTemp	= NULL;

	MotionBlur* pMotionBlur = new MotionBlur();
	m_pPostEffects.push_back(pMotionBlur);
	m_pDevice=Renderer::GetInstance()->m_pd3dDevice;

	m_PostEffects = 0;
}

//----------------------------------------------------------------------------------------------
PostRenderer::~PostRenderer(void)
{
	Release ();

	for (u32 postEffect = 0; postEffect < m_pPostEffects.size(); postEffect++)
	{
		if (m_pPostEffects[postEffect])
			delete m_pPostEffects[postEffect];
	}
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
	m_pDevice->BeginScene();

	for (u32 postEffect = 0; postEffect < m_pPostEffects.size(); postEffect++)
	{
		if (m_pPostEffects[postEffect] && ((m_PostEffects & (1<<postEffect)) == 1<<postEffect))
			m_pPostEffects[postEffect]->Apply(&SceneObject::RefList);
	}

	if (! (m_pSceneRenderTarget && m_pBackBuffer))
		return;

	RECT windowRect;
	windowRect.left		= 0;
	windowRect.top		= 0;
	windowRect.right	= Renderer::GetInstance()->GetWindowWidth();
	windowRect.bottom	= Renderer::GetInstance()->GetWindowHeight();
	m_pDevice->StretchRect(m_pSceneRenderTarget->GetSurface(), NULL, m_pBackBuffer, &windowRect, D3DTEXF_NONE);

	m_pDevice->EndScene();
}

//----------------------------------------------------------------------------------------------
void PostRenderer::Release ()
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

	if (m_pBackBuffer)
	{
		m_pBackBuffer->Release();
		m_pBackBuffer = NULL;
	}
//*
	for (u32 postEffect = 0; postEffect < m_pPostEffects.size(); postEffect++)
	{
		if (m_pPostEffects[postEffect])
			m_pPostEffects[postEffect]->Release();
	}
//*/
}

//----------------------------------------------------------------------------------------------
void PostRenderer::Destroy ()
{
	for (u32 postEffect = 0; postEffect < m_pPostEffects.size(); postEffect++)
	{
		if (m_pPostEffects[postEffect])
			m_pPostEffects[postEffect]->Destroy();
	}
}

//----------------------------------------------------------------------------------------------
HRESULT PostRenderer::Create (LPDIRECT3DDEVICE9 _pDevice, u32 _width, u32 _height)
{

	//// ToDo : Change return value
	assert (_pDevice);
	
	//HRESULT renderTargetCrateation;

	m_pSceneRenderTarget		= new RenderTarget(_width, _height);
	m_pSceneRenderTarget->Create(_pDevice);

	m_pSceneRenderTargetTemp	= new RenderTarget(_width, _height);
	m_pSceneRenderTargetTemp->Create(_pDevice);
//*
	for (u32 postEffect = 0; postEffect < m_pPostEffects.size(); postEffect++)
	{
		if (m_pPostEffects[postEffect])
			m_pPostEffects[postEffect]->Create(_pDevice, _width, _height);
	}
//*/
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

	return NULL;
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

	return NULL;
}
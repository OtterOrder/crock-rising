#include "VectorialBlur.h"

#include "Resources/ResourceManager.h"

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
VectorialBlur::VectorialBlur(void)
{
	SetShader("VectorialBlur.fx");

	m_pTextureToBlur	= NULL;
	m_pVectorsTexture	= NULL;
	m_pBlurredSurface	= NULL;

	m_pCurrentRenderTarget	= NULL;
}

//----------------------------------------------------------------------------------------------
VectorialBlur::~VectorialBlur(void)
{
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void VectorialBlur::Create (LPDIRECT3DDEVICE9 _pDevice, u32 _width, u32 _height)
{
	if (!m_pShader)
		return;
	m_pShader->GetEffect()->OnResetDevice();
	
	m_pRenderQuad = new Quad (10, 10, Color4f(0.f, 1.f, 0.f, 1.f));
	Object2D::RefList.remove(m_pRenderQuad);
}

//----------------------------------------------------------------------------------------------
void VectorialBlur::Release ()
{
	if (!m_pShader)
		return;
	m_pShader->GetEffect()->OnLostDevice();
	
	if(m_pRenderQuad)
	{
		delete m_pRenderQuad;
		m_pRenderQuad = NULL;
	}
}

//----------------------------------------------------------------------------------------------
void VectorialBlur::Destroy ()
{
	ResourceManager::GetInstance()->Remove<Shader>("VectorialBlur.fx");
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void VectorialBlur::Apply(list<SceneObject*>* m_pObjectList)
{
	if (!(m_pTextureToBlur && m_pVectorsTexture && m_pBlurredSurface))
		return;

	// Adapts Quad Size
	D3DSURFACE_DESC surfaceToBlurDesc;
	m_pTextureToBlur->GetLevelDesc(0, &surfaceToBlurDesc);
	m_pRenderQuad->SetSize(surfaceToBlurDesc.Width, surfaceToBlurDesc.Height);

	// Change Render target;
	m_pCurrentRenderTarget = PostRenderer::GetInstance()->GetRenderTarget();
	PostRenderer::GetInstance()->SetRenderTarget(m_pBlurredSurface);

	// Parameter shader
	m_pShader->GetEffect()->SetTexture( "g_TextureToBlur",	m_pTextureToBlur);
	m_pShader->GetEffect()->SetTexture( "g_VectorsTexture",	m_pVectorsTexture);

	// Apply the shader
	m_pShader->GetEffect()->SetTechnique( "VectorialBlur" );

	m_pShader->GetEffect()->Begin(0, 0);

	m_pShader->GetEffect()->BeginPass(0);

	m_pRenderQuad->Draw();

	m_pShader->GetEffect()->EndPass();

	m_pShader->GetEffect()->End();

	// Restore Render target
	PostRenderer::GetInstance()->SetRenderTarget(m_pCurrentRenderTarget);
	m_pCurrentRenderTarget->Release();
}
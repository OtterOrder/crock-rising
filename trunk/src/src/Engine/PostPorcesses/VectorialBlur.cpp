#include "VectorialBlur.h"

VectorialBlur::VectorialBlur(void)
{
	SetShader("VectorialBlur.fx");

	m_pTextureToBlur	= NULL;
	m_pVectorsTexture	= NULL;
	m_pBlurredSurface	= NULL;

	m_pSurfaceToBlur	= NULL;
	m_pRenderQuad = new Quad (10, 10, Color4f(0.f, 1.f, 0.f, 1.f));
	Object2D::RefList.remove(m_pRenderQuad);

	m_pCurrentRenderTarget	= NULL;
}

VectorialBlur::~VectorialBlur(void)
{
}

void VectorialBlur::Apply(list<SceneObject*>* m_pObjectList)
{
	if (!(m_pTextureToBlur && m_pVectorsTexture && m_pBlurredSurface))
		return;

	// Adapts Quad Size
	m_pTextureToBlur->GetSurfaceLevel(0, &m_pSurfaceToBlur);
	D3DSURFACE_DESC surfaceToBlurDesc;
	m_pSurfaceToBlur->GetDesc(&surfaceToBlurDesc);
	m_pRenderQuad->SetSize(surfaceToBlurDesc.Width, surfaceToBlurDesc.Height);

	// Change Render target;
	m_pCurrentRenderTarget = PostRenderer::GetInstance()->GetRenderTarget();
	PostRenderer::GetInstance()->SetRenderTarget(m_pBlurredSurface);

	// Parameter shader
	m_pShader->m_pEffect->SetTexture( "g_TextureToBlur",	m_pTextureToBlur);
	m_pShader->m_pEffect->SetTexture( "g_VectorsTexture",	m_pVectorsTexture);


	// Apply the shader
	m_pShader->m_pEffect->SetTechnique( "VectorialBlur" );

	m_pShader->m_pEffect->Begin(0, 0);

	m_pShader->m_pEffect->BeginPass(0);

	m_pRenderQuad->Draw();

	m_pShader->m_pEffect->EndPass();

	m_pShader->m_pEffect->End();

	// Restore Render target
	PostRenderer::GetInstance()->SetRenderTarget(m_pCurrentRenderTarget);
}
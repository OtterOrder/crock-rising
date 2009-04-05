#include "MotionBlur.h"

#include "Resources/Mesh.h"

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
MotionBlur::MotionBlur(void)
{
	SetShader("motionBlurVelocity.fx");

	D3DXMatrixIdentity(&m_ViewProj);
	D3DXMatrixIdentity(&m_LastViewProj);

	m_pVelocity = NULL;
}

//----------------------------------------------------------------------------------------------
MotionBlur::MotionBlur(u32 _width, u32 _height)
{
	SetShader("motionBlurVelocity.fx");

	D3DXMatrixIdentity(&m_ViewProj);
	D3DXMatrixIdentity(&m_LastViewProj);

	m_pVelocity = new RenderTarget(_width, _height);
	m_pVelocity->Create(Renderer::GetInstance()->m_pd3dDevice);
}

//----------------------------------------------------------------------------------------------
MotionBlur::~MotionBlur(void)
{
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void MotionBlur::Apply(list<SceneObject*>* _pObjectList)
{
	if (!_pObjectList)
		return;

	RenderVelocity (_pObjectList);

	PostRenderer::GetInstance()->SetRenderTarget(PostRenderer::GetInstance()->GetSceneRenderSurfaceTemp());

	VectorialBlur* pVectorialBlur = VectorialBlur::GetInstance();
	// blur
	pVectorialBlur->m_pTextureToBlur	= PostRenderer::GetInstance()->GetSceneRenderTexture();
	pVectorialBlur->m_pVectorsTexture	= m_pVelocity->GetTexture();
	pVectorialBlur->m_pBlurredSurface	= PostRenderer::GetInstance()->GetSceneRenderSurfaceTemp();

	pVectorialBlur->Apply();

	PostRenderer::GetInstance()->SwapSceneRender();
}

//----------------------------------------------------------------------------------------------
void MotionBlur::RenderVelocity (list<SceneObject*>* _pObjectList)
{
	Mesh* pMesh = NULL;

	m_ViewProj = Renderer::GetInstance()->GetViewProjMatrix();

	D3DXMATRIX WorldViewProj;
	D3DXMATRIX LastWorldViewProj;

	// set render target
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetInstance()->m_pd3dDevice;
	PostRenderer::GetInstance()->SetRenderTarget(m_pVelocity->GetSurface());
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_COLOR4F( Color4f(0.f, 0.f, 0.f, 1.f)), 1.0f, 0);

	SceneObject::ScObjIt scobj;
	scobj = _pObjectList->begin();
	while( scobj != _pObjectList->end() )
	{
		pMesh = (*scobj)->GetMesh();

		D3DXMatrixMultiply(&WorldViewProj		,(*scobj)->GetWorldMatrix(), &m_ViewProj);
		D3DXMatrixMultiply(&LastWorldViewProj	,(*scobj)->GetWorldMatrix(), &m_LastViewProj);

		m_pShader->m_pEffect->SetMatrix( "g_mWorldViewProjection",	&WorldViewProj);
		m_pShader->m_pEffect->SetMatrix( "g_mWorldViewProjLast",	&LastWorldViewProj);

		// Draw Objects
		pDevice->SetVertexDeclaration(pMesh->m_decl);

		m_pShader->m_pEffect->SetTechnique( "RenderVelocity" );

		m_pShader->m_pEffect->Begin(0, 0);

		m_pShader->m_pEffect->BeginPass(0);

		if (pMesh->m_pVB)
		{
			pDevice->SetStreamSource(0, pMesh->m_pVB, 0, D3DXGetDeclVertexSize(pMesh->m_VBdecl, 0));

			pDevice->SetIndices ( pMesh->m_pIB );

			pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pMesh->m_iNbVertices, 0, pMesh->m_iNbIndex/3); 
		}

		m_pShader->m_pEffect->EndPass();

		m_pShader->m_pEffect->End();

		scobj ++;
	}

	m_LastViewProj = m_ViewProj;

	// Reinit RenderTarget
	PostRenderer::GetInstance()->SetRenderTarget(PostRenderer::GetInstance()->GetSceneRenderSurface());
}
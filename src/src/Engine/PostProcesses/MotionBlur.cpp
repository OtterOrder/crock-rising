#include "MotionBlur.h"

#include "Resources/Mesh.h"
#include "Resources/ResourceManager.h"

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
MotionBlur::MotionBlur(void)
{
	m_pShader = NULL;
	SetShader("motionBlurVelocity.fx");

	D3DXMatrixIdentity(&m_ViewProj);
	D3DXMatrixIdentity(&m_LastViewProj);

	m_pVelocity = NULL;
}

//----------------------------------------------------------------------------------------------
MotionBlur::~MotionBlur(void)
{
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void MotionBlur::Create (LPDIRECT3DDEVICE9 _pDevice, u32 _width, u32 _height)
{
	m_pVelocity = new RenderTarget(_width, _height);
	m_pVelocity->Create(_pDevice);

	if (!m_pShader)
		return;
	m_pShader->GetEffect()->OnResetDevice();

	VectorialBlur::GetInstance()->Create();
}

//----------------------------------------------------------------------------------------------
void MotionBlur::Release ()
{
	if (m_pVelocity)
	{
		delete m_pVelocity;
		m_pVelocity = NULL;
	}

	if (!m_pShader)
		return;
	m_pShader->GetEffect()->OnLostDevice();

	VectorialBlur::GetInstance()->Release();
}

//----------------------------------------------------------------------------------------------
void MotionBlur::Destroy ()
{
	ResourceManager::GetInstance()->Remove<Shader>("motionBlurVelocity.fx");

	VectorialBlur::GetInstance()->Destroy();
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

		m_pShader->GetEffect()->SetMatrix( "g_mWorldViewProjection",	&WorldViewProj);
		m_pShader->GetEffect()->SetMatrix( "g_mWorldViewProjLast",	&LastWorldViewProj);

		// Draw Objects
		pDevice->SetVertexDeclaration(pMesh->m_decl);

		m_pShader->GetEffect()->SetTechnique( "RenderVelocity" );

		m_pShader->GetEffect()->Begin(0, 0);

		m_pShader->GetEffect()->BeginPass(0);

		if (pMesh->m_pVB)
		{
			pDevice->SetStreamSource(0, pMesh->m_pVB, 0, D3DXGetDeclVertexSize(pMesh->m_VBdecl, 0));

			pDevice->SetIndices ( pMesh->m_pIB );

			pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pMesh->m_iNbVertices, 0, pMesh->m_iNbIndex/3); 
		}

		m_pShader->GetEffect()->EndPass();

		m_pShader->GetEffect()->End();

		scobj ++;
	}

	m_LastViewProj = m_ViewProj;

	// Reinit RenderTarget
	PostRenderer::GetInstance()->SetRenderTarget(PostRenderer::GetInstance()->GetSceneRenderSurface());
}
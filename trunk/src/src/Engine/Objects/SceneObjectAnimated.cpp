#include	"SceneObjectAnimated.h"
#include	"Resources/ResourceManager.h"

#include	"Resources/Mesh.h"
#include	"Resources/Shader.h"
#include	"Resources/Anim.h"

SceneObjectAnimated::SceneObjectAnimated()
:SceneObject()
{
}

SceneObjectAnimated::SceneObjectAnimated(const std::string& mesh,
										 const std::string& anim,
										 const std::string& Tex,
										 const D3DXVECTOR3& Position,
										 const std::string shader,
										 BoundingBox aBoundingBox)
:SceneObject(mesh, Tex, Position, shader, aBoundingBox)
{
	m_Anim = anim ;
	m_pAnim = NULL;
}

void SceneObjectAnimated::InitObjectAnimation()
{
	InitObject();
	m_pAnim = ResourceManager::GetInstance()->Load<Anim>(m_Anim);

	m_AnimMatrices = new D3DXMATRIX[22];
}

void SceneObjectAnimated::Draw()
{

	m_pDevice->SetVertexDeclaration(m_PtrMesh->m_decl);

	m_PtrShader->m_pEffect->SetTechnique( "RenderSceneNoTex" );

	/*
	D3DXMATRIX l_MatricesTest [22];
	m_PtrShader->m_pEffect->SetMatrixArray("g_skinningMatrices", l_MatricesTest, 22);
	*/

	if (m_pAnim)
	{
		//D3DXMATRIX l_MatricesTest [22];
		//D3DXMATRIX* l_MatricesTest = new D3DXMATRIX[22];

		for(int m=0 ; m<22 ; m++)
		{
			for (int i=0 ; i<4 ; i++)
			{
				for (int j=0 ; j<4 ; j++)
					;//m_AnimMatrices[m](i,j) = m_pAnim->m_fBonesMatrice[m][0][i][j];
			}

			 D3DXMatrixIdentity(&m_AnimMatrices[m]);
		}

		m_PtrShader->m_pEffect->SetMatrixArray("g_skinningMatrices", m_AnimMatrices, 22);
	}

	m_PtrShader->m_pEffect->Begin(0, 0);

	m_PtrShader->m_pEffect->BeginPass(0);

	if (m_PtrMesh->m_pVB)
	{
		m_pDevice->SetStreamSource(0, m_PtrMesh->m_pVB, 0, sizeof(Vertex));

		m_pDevice->SetIndices ( m_PtrMesh->m_pIB );

		m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_PtrMesh->m_iNbVertices, 0, m_PtrMesh->m_iNbIndex/3); 
	}

	m_PtrShader->m_pEffect->EndPass();

	m_PtrShader->m_pEffect->End();

}

void SceneObjectAnimated::SetTransform(const D3DXMATRIX* world, const D3DXMATRIX* view, const D3DXMATRIX* proj)
{
	D3DXMATRIX MatWorldView;
	D3DXMATRIX mWorldViewProjection;
	D3DXMATRIX mViewProjection;
	D3DXMatrixIdentity(&MatWorldView);
	D3DXMatrixMultiply(&m_WorldMatrix, &m_WorldMatrix, world);
	D3DXMatrixMultiply(&MatWorldView, &m_WorldMatrix, view);
	D3DXMatrixMultiply(&mWorldViewProjection, &MatWorldView, proj);
	D3DXMatrixMultiply(&mViewProjection, view, proj);

	m_PtrShader->m_pEffect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection);
	m_PtrShader->m_pEffect->SetMatrix( "g_mWorld", &m_WorldMatrix);
	m_PtrShader->m_pEffect->SetMatrix( "g_mView", view);
	D3DXMATRIXA16 mWorldView = (*m_WorldMatrix) * (*view);
	m_PtrShader->m_pEffect->SetMatrix( "g_mWorldView", &MatWorldView);
	m_PtrShader->m_pEffect->SetMatrix( "g_mViewProj", &mViewProjection);

	D3DXMATRIX mIndentity;
	D3DXMatrixIdentity(&mIndentity);
	m_PtrShader->m_pEffect->SetMatrix( "g_mIdentity", &mIndentity);
}

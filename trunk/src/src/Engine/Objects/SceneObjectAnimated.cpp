#include	"SceneObjectAnimated.h"
#include	"Resources/ResourceManager.h"

#include	"Resources/Mesh.h"
#include	"Resources/Shader.h"
#include	"Resources/Anim.h"

//#define __NB_BONES	25
//#define	__NB_FRAME	210
#define __NB_BONES	22
#define	__NB_FRAME	61

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

	m_AnimMatrices = new D3DXMATRIX[__NB_BONES];

	m_curFrame = 0;
}

void SceneObjectAnimated::Draw()
{

	m_pDevice->SetVertexDeclaration(m_PtrMesh->m_decl);

	m_PtrShader->m_pEffect->SetTechnique( "RenderSceneNoTex" );

	D3DXMATRIX skinBindShape;
	for (int i=0 ; i<4 ; i++)
			for (int j=0 ; j<4 ; j++)
				skinBindShape(i,j) = m_pAnim->m_fBindShapeMatrix[i][j];

	if (m_pAnim)
	{
		m_curFrame ++;

		if (m_curFrame >= __NB_FRAME)
			m_curFrame = 0;

		for(int m=0 ; m<__NB_BONES ; m++)
		{
			for (int i=0 ; i<4 ; i++)
			{
				for (int j=0 ; j<4 ; j++)
					m_AnimMatrices[m](i,j) = m_pAnim->m_fBonesMatrice[m][m_curFrame][i][j];
			}

			//D3DXMatrixMultiply(&m_AnimMatrices[m], &m_AnimMatrices[m], &skinBindShape);
			//m_AnimMatrices[m].

			D3DXMATRIX skinBindPoses;
			for (int i=0 ; i<4 ; i++)
					for (int j=0 ; j<4 ; j++)
						skinBindPoses(i,j) = m_pAnim->m_fBindPosesArray[m][i][j];

			//D3DXMatrixInverse(&skinBindPoses, NULL,&skinBindPoses);
			D3DXMatrixMultiply(&skinBindPoses, &skinBindPoses, &skinBindShape);

			m_AnimMatrices[m] = skinBindPoses;
			//D3DXMatrixMultiply(&m_AnimMatrices[m], &skinBindPoses, &m_AnimMatrices[m]);
			//D3DXMatrixMultiply(&m_AnimMatrices[m], &m_AnimMatrices[m], &skinBindPoses);
		
			//D3DXMatrixIdentity(&m_AnimMatrices[m]);
		}

		m_PtrShader->m_pEffect->SetMatrixArray("g_skinningMatrices", m_AnimMatrices, __NB_BONES);
	}

	m_PtrShader->m_pEffect->Begin(0, 0);

	m_PtrShader->m_pEffect->BeginPass(0);

	if (m_PtrMesh->m_pVB)
	{
		m_pDevice->SetStreamSource(0, m_PtrMesh->m_pVB, 0, sizeof(SkinnedVertex));

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

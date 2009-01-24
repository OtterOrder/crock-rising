#include "SceneObject.h"
#include "../Resources/ResourceManager.h"
#include	"Renderer/Renderer.h"

//===========================================================================//
// Constructeurs SceneObject		                                         //
//===========================================================================//
SceneObject::SceneObject()
{
}

SceneObject::SceneObject(const std::string& mesh, const std::string& Tex, const D3DXVECTOR3& Position, const std::string shader)
{
	m_Mesh=mesh;
	m_Tex=Tex;
	m_Offset=Position;
	D3DXMatrixIdentity(&m_WorldMatrix);

	m_Shader = shader;
}

//===========================================================================//
// Ajoute une texture à la map		                                         //
//===========================================================================//
void SceneObject::SetTexture(const std::string& Tex, types_tex Type)
{
	// On teste si la resource existe déjà
	m_MapTexture[Type]=ResourceManager::GetInstance()->Load<Texture>(Tex);

	/*switch (Type)
	{
		case NORMALMAP :
			   m_SceneShader->m_pEffect->SetTexture("g_NormalMap", m_MapTexture[NORMALMAP]->m_pTex);
			   break;
	}*/
}

//===========================================================================//
// Attribue un shader à l'objet 										     //
//===========================================================================//
void SceneObject::SetShader(const std::string& Shad)
{
	m_PtrShader=ResourceManager::GetInstance()->Load<Shader>(Shad);
}

void SceneObject::InitObject()
{
	m_PtrMesh=ResourceManager::GetInstance()->Load<Mesh>(m_Mesh);
	m_MapTexture[MESHTEX]=ResourceManager::GetInstance()->Load<Texture>(m_Tex);
	m_PtrShader=ResourceManager::GetInstance()->Load<Shader>(m_Shader.c_str());

	m_PtrShader->m_pEffect->SetTexture("g_MeshTexture", m_MapTexture[MESHTEX]->m_pTex);

	// Matrice World par défaut de l'objet
	D3DXMATRIX WorldMatrix;
	D3DXMATRIX Translate;
	D3DXMatrixTranslation(&Translate, m_PtrMesh->m_Position.x+m_Offset.x, m_PtrMesh->m_Position.y+m_Offset.y, m_PtrMesh->m_Position.z+m_Offset.z); 
	D3DXMATRIX Rotate;
	D3DXMatrixRotationAxis(&Rotate, &D3DXVECTOR3(m_PtrMesh->m_Rotation.x, m_PtrMesh->m_Rotation.y, m_PtrMesh->m_Rotation.z), D3DXToRadian(m_PtrMesh->m_Rotation.w));  
	D3DXMATRIX Scale;
	D3DXMatrixScaling(&Scale, m_PtrMesh->m_Scale.x, m_PtrMesh->m_Scale.y, m_PtrMesh->m_Scale.z);

	D3DXMATRIX Transform;
	D3DXMatrixMultiply(&Transform, &Rotate, &Translate); 
	D3DXMatrixMultiply(&WorldMatrix, &Transform, &Scale); 
	D3DXMatrixMultiply(&m_WorldMatrix, &m_WorldMatrix, &WorldMatrix); 
}

//===========================================================================//
// Effectue une transformation				                                 //
//===========================================================================//
void SceneObject::SetTransform(const D3DXMATRIX *world)
{

}

void SceneObject::SetTransform(const D3DXMATRIX* world, const D3DXMATRIX* view, const D3DXMATRIX* proj)
{
	D3DXMATRIX MatWorldView;
	D3DXMATRIX mWorldViewProjection;
	D3DXMatrixIdentity(&MatWorldView);
	D3DXMatrixMultiply(&m_WorldMatrix, &m_WorldMatrix, world);
	D3DXMatrixMultiply(&MatWorldView, &m_WorldMatrix, view);
	D3DXMatrixMultiply(&mWorldViewProjection, &MatWorldView, proj);

	m_PtrShader->m_pEffect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection);
	m_PtrShader->m_pEffect->SetMatrix( "g_mWorld", &m_WorldMatrix);
	m_PtrShader->m_pEffect->SetMatrix( "g_mView", view);
	D3DXMATRIXA16 mWorldView = (*m_WorldMatrix) * (*view);
	m_PtrShader->m_pEffect->SetMatrix( "g_mWorldView", &MatWorldView);

}

void SceneObject::SetTransform(const D3DXMATRIX* world, const D3DXMATRIX* view, const D3DXMATRIX* proj, const D3DXVECTOR3 CamPos)
{
	D3DXMATRIX MatWorldView;
	D3DXMATRIX mWorldViewProjection;
	D3DXMatrixIdentity(&MatWorldView);
	D3DXMatrixMultiply(&m_WorldMatrix, &m_WorldMatrix, world);
	D3DXMatrixMultiply(&MatWorldView, &m_WorldMatrix, view);
	D3DXMatrixMultiply(&mWorldViewProjection, &MatWorldView, proj);

	m_PtrShader->m_pEffect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection);
	m_PtrShader->m_pEffect->SetMatrix( "g_mWorld", &m_WorldMatrix);
	m_PtrShader->m_pEffect->SetMatrix( "g_mView", view);
	D3DXMATRIXA16 mWorldView = (*m_WorldMatrix) * (*view);
	m_PtrShader->m_pEffect->SetMatrix( "g_mWorldView", &MatWorldView);

	m_PtrShader->m_pEffect->SetValue("g_vCamPos", CamPos, sizeof(D3DXVECTOR3));

}

//===========================================================================//
// Gestion de l'objet						                                 //
//===========================================================================//
void SceneObject::InitDeviceData()
{
	m_PtrMesh->FillD3DBuffers();
	m_PtrShader->m_pEffect->OnResetDevice();
}

void SceneObject::FrameMove(float fElapsedTime)
{

}

void SceneObject::Draw()
{
	m_pDevice = Renderer::GetInstance()->m_pd3dDevice;

	m_pDevice->BeginScene();

	m_pDevice->SetVertexDeclaration(m_PtrMesh->m_decl);

	m_PtrShader->m_pEffect->SetTechnique( "RenderSceneNoTex" );

	m_PtrShader->m_pEffect->Begin(0, 0);

	m_PtrShader->m_pEffect->BeginPass(0);

	if (m_PtrMesh->m_pVB)
	{
			//m_pDevice->SetFVF(Mesh_FVF);
			
			m_pDevice->SetStreamSource(0, m_PtrMesh->m_pVB, 0, sizeof(Vertex));

			m_pDevice->SetIndices ( m_PtrMesh->m_pIB );

			m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_PtrMesh->m_iNbVertices, 0, m_PtrMesh->m_iNbIndex/3); 
	}

	m_PtrShader->m_pEffect->EndPass();

	m_PtrShader->m_pEffect->End();

	m_pDevice->EndScene();

}

void SceneObject::DeleteDeviceData()
{
	m_PtrMesh->ReleaseD3DBuffers();
	m_PtrShader->m_pEffect->OnLostDevice();
}

void SceneObject::DeleteData()
{
	m_PtrMesh->Release();

	/*for(TTextureMap::const_iterator it=m_MapTexture.begin() ; it!=m_MapTexture.end() ; ++it)
	{
			it->second->Release();
	}*/
	m_PtrShader->Release();

}

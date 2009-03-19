#include	"SceneObject.h"

#include	"Renderer/Renderer.h"
#include	"Resources/ResourceManager.h"
#include	"Resources/Shader.h"
#include	"Resources/Mesh.h"
#include	"../Physics/BoundingBox.h"

using namespace std;

//******************************************************************

list< SceneObject* > SceneObject::RefList;

//******************************************************************

//===========================================================================//
// Initialisation commune à tous les constructeurs.                          //
//===========================================================================//
void SceneObject::CommonInit( void )
{
	SceneObject::RefList.push_front( this ); // enregistrement dans la liste
}

//===========================================================================//
// Constructeurs & destructeur de SceneObject		                         //
//===========================================================================//
SceneObject::SceneObject()
:Object()
{
	CommonInit();
}
//Note : Le parametre position définis l'origine de référence de l'objet qui
//sera utilisé par le moteur physique. Il convient bien de mettre 0,0,0.
//à approfondir pour les autres valeurs. La véritable position dans l'espace sera
//alors donné par la boundingbox.
SceneObject::SceneObject(const std::string& mesh,
						 const std::string& Tex,
						 const D3DXVECTOR3& Position,
						 const std::string shader)
:Object(Position)
{	
	CommonInit();

	m_Mesh=mesh;
	m_Tex=Tex;
	m_Offset=Position;

	m_Shader = shader;
	m_EmpList = new TEmpList;
}

SceneObject::~SceneObject()
{
	m_EmpList->clear(); // suppression dans la liste
	SceneObject::RefList.remove( this ); // suppression dans la liste
}

//===========================================================================//
// Ajoute une texture à la map		                                         //
//===========================================================================//
void SceneObject::SetTexture(const std::string& Tex, TextureType Type)
{
	// On teste si la resource existe déjà
	m_MapTexture[Type]=ResourceManager::GetInstance()->Load<Texture>(Tex);

	switch (Type)
	{
		case TEX_MESH :
			   m_PtrShader->m_pEffect->SetTexture("g_MeshTexture", m_MapTexture[TEX_MESH]->m_pTex);
			   break;

		/*case NORMALMAP :
			   m_PtrShader->m_pEffect->SetTexture("g_NormalMap", m_MapTexture[NORMALMAP]->m_pTex);
			   break;*/
	}
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
	m_pDevice = Renderer::GetInstance()->m_pd3dDevice;
		
	m_PtrMesh = ResourceManager::GetInstance()->Load<Mesh>(m_Mesh);
	m_MapTexture[TEX_MESH]=ResourceManager::GetInstance()->Load<Texture>(m_Tex);
	m_PtrShader=ResourceManager::GetInstance()->Load<Shader>(m_Shader.c_str());


	//m_PtrShader->m_pEffect->SetTexture("g_MeshTexture", m_MapTexture[TEX_MESH]->m_pTex);

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
	m_WorldMatrix *= *world; //tranformation s'accumule
}

void SceneObject::SetTransform(const D3DXMATRIX* world, const D3DXMATRIX* view, const D3DXMATRIX* proj)
{
	D3DXMATRIX MatWorldView;
	D3DXMATRIX mWorldViewProjection;
	D3DXMATRIX Scale;

	D3DXMatrixScaling(&Scale, m_PtrMesh->m_Scale.x, m_PtrMesh->m_Scale.y, m_PtrMesh->m_Scale.z);
	D3DXMatrixIdentity(&MatWorldView);
	D3DXMatrixMultiply(&m_WorldMatrix, &m_WorldMatrix, &Scale);
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
	m_pDevice->SetVertexDeclaration(m_PtrMesh->m_decl);

	m_PtrShader->m_pEffect->SetTexture("g_MeshTexture", m_MapTexture[TEX_MESH]->m_pTex);

	m_PtrShader->m_pEffect->SetTechnique( "RenderScene" );

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

void SceneObject::DeleteDeviceData()
{
	m_PtrMesh->ReleaseD3DBuffers();
	m_PtrShader->m_pEffect->OnLostDevice();
}

void SceneObject::DeleteData()
{
	ResourceManager::GetInstance()->Remove<Mesh>(m_Mesh);
	ResourceManager::GetInstance()->Remove<Shader>(m_Shader);
	//m_PtrMesh->Release();

	for(TTextureMap::const_iterator it=m_MapTexture.begin() ; it!=m_MapTexture.end() ; ++it)
	{
			ResourceManager::GetInstance()->Remove<Texture>(it->second->GetName());
			//it->second->Release();
	}
	//m_PtrShader->Release();
}
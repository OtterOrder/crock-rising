#include "SceneObject.h"

#include "Renderer/Renderer.h"
#include "Resources/ResourceManager.h"
#include "Resources/Shader.h"
#include "Resources/Mesh.h"
#include "Resources/Material.h"
#include "../Physics/BoundingBox.h"

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
	m_pMaterial=NULL;
	CommonInit();
}
//Note : Le parametre position définis l'origine de référence de l'objet qui
//sera utilisé par le moteur physique. Il convient bien de mettre 0,0,0.
//à approfondir pour les autres valeurs. La véritable position dans l'espace sera
//alors donné par la boundingbox.
SceneObject::SceneObject(const std::string& mesh,
						 const D3DXVECTOR3& Position)
:Object(Position)
{	
	CommonInit();
	m_strShader="default.fx";
	m_strMesh=mesh;
	m_Offset=Position;
	m_pMaterial=NULL;

	m_WorldMatrix._22=0; m_WorldMatrix._23=1;
	m_WorldMatrix._32=1; m_WorldMatrix._33=0;

	m_EmpList = new TEmpList;
}

SceneObject::~SceneObject()
{
	m_EmpList->clear(); // suppression dans la liste
	if(m_pMaterial)
		delete m_pMaterial;
}

//===========================================================================//
// Gestion matériaux				                                         //
//===========================================================================//
void SceneObject::SetMaterial(Material * mat)
{
	m_pMaterial=mat;
}

//===========================================================================//
// Attribue un shader à l'objet 										     //
//===========================================================================//
void SceneObject::SetShader(const std::string& strShader)
{
	if(m_pShader)
	{
		ResourceManager::GetInstance()->Remove<Shader>(m_strShader);
		m_pShader=ResourceManager::GetInstance()->Load<Shader>(strShader.c_str());
		m_pMaterial->SetShader(m_pShader);
	}
}

void SceneObject::InitObject()
{
	m_pDevice = Renderer::GetInstance()->m_pd3dDevice;

	m_pMesh=ResourceManager::GetInstance()->Load<Mesh>(m_strMesh);
	m_pShader=ResourceManager::GetInstance()->Load<Shader>(m_strShader.c_str());

	// Matériau par défaut de l'objet
	m_pMaterial=new Material();

	// On assigne le shader au matériau
	m_pMaterial->SetShader(m_pShader);

	// On crée le stack de matrices
	D3DXCreateMatrixStack( 0, &m_matrixStack );

	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, m_Offset.x, m_Offset.y, m_Offset.z);
	D3DXMatrixMultiply(&m_WorldMatrix, &m_WorldMatrix, &trans);
}

//===========================================================================//
// Effectue une transformation				                                 //
//===========================================================================//
void SceneObject::SetTransform(const D3DXMATRIX* view, const D3DXMATRIX* proj)
{
	D3DXMATRIX MatWorldView;
	D3DXMATRIX mWorldViewProjection;
	D3DXMatrixIdentity(&MatWorldView);
	D3DXMatrixMultiply(&MatWorldView, &m_WorldMatrix, view);
	D3DXMatrixMultiply(&mWorldViewProjection, &MatWorldView, proj);

	m_pShader->m_pEffect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection);
	m_pShader->m_pEffect->SetMatrix( "g_mWorld", &m_WorldMatrix);
	m_pShader->m_pEffect->SetMatrix( "g_mView", view);
	D3DXMATRIXA16 mWorldView = (*m_WorldMatrix) * (*view);
	m_pShader->m_pEffect->SetMatrix( "g_mWorldView", &MatWorldView);
}

void SceneObject::SetTransform(const D3DXMATRIX* view, const D3DXMATRIX* proj, const D3DXVECTOR3 CamPos)
{
	SceneObject::SetTransform(view, proj);
	m_pShader->m_pEffect->SetValue("g_vCamPos", CamPos, sizeof(D3DXVECTOR3));
}

void SceneObject::SetTransform(const D3DXMATRIX* world)
{
	D3DXMatrixMultiply(&m_WorldMatrix, &m_WorldMatrix, world);
}

void SceneObject::ApplyTransform(const D3DXMATRIX *world)
{
	m_matrixStack->LoadIdentity();
    m_matrixStack->LoadMatrix( &m_WorldMatrix );
	m_matrixStack->Push();
    {
        m_matrixStack->MultMatrixLocal( world );
		m_WorldMatrix=*m_matrixStack->GetTop();
    }
    m_matrixStack->Pop();
}

//===========================================================================//
// Gestion de l'objet						                                 //
//===========================================================================//
void SceneObject::InitDeviceData()
{
	m_pMesh->FillD3DBuffers();
	m_pShader->m_pEffect->OnResetDevice();
}

void SceneObject::Draw()
{
	m_pDevice->SetVertexDeclaration(m_pMesh->m_decl);

	m_pMaterial->SetGraphicalData();

	m_pShader->m_pEffect->SetTechnique( "RenderScene" );

	m_pShader->m_pEffect->Begin(0, 0);

	m_pShader->m_pEffect->BeginPass(0);

	if (m_pMesh->m_pVB)
	{
			m_pDevice->SetStreamSource(0, m_pMesh->m_pVB, 0, D3DXGetDeclVertexSize(m_pMesh->m_VBdecl, 0));

			m_pDevice->SetIndices ( m_pMesh->m_pIB );

			m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_pMesh->m_iNbVertices, 0, m_pMesh->m_iNbIndex/3); 
	}

	m_pShader->m_pEffect->EndPass();

	m_pShader->m_pEffect->End();

}

void SceneObject::DeleteDeviceData()
{
	m_pMesh->ReleaseD3DBuffers();
	m_pShader->m_pEffect->OnLostDevice();
}

void SceneObject::DeleteData()
{
	ResourceManager::GetInstance()->Remove<Mesh>(m_strMesh);
	ResourceManager::GetInstance()->Remove<Shader>(m_strShader);

	if( m_matrixStack != NULL )
        m_matrixStack->Release();

	m_pMaterial->DeleteData();
}

//===========================================================================//
// Accesseurs                					                             //
//===========================================================================//
void SceneObject::SetVisible(bool value)	{m_bVisible = value;}
bool SceneObject::GetVisible()				{return m_bVisible;}
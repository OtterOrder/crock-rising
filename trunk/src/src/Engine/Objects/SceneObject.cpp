#include "SceneObject.h"

#include "Renderer/Renderer.h"
#include "Resources/ResourceManager.h"
#include "Resources/Shader.h"
#include "Resources/Mesh.h"
#include "Resources/Material.h"
#include "Renderer/Shadow Map/ShadowMap.h"
#include "../Physics/Physicalizer.h"

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

	m_iEmpActor = -1;
	m_iEmpController = -1;

	m_bVisible=true;
	m_bCastShadow = false;
	m_bReceiveShadow = false;
}

SceneObject::~SceneObject()
{
	m_ListOfBoundingBox.ReleaseList(); // suppression dans la liste
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
		m_strShader=strShader;
		m_pShader=ResourceManager::GetInstance()->Load<Shader>(strShader.c_str());
		m_pMaterial->SetShader(m_pShader);
	}
}

//===========================================================================//
// Physique de l'objet														 //
//===========================================================================//
void SceneObject::SetObjectPhysical( const std::string& physic, GroupCollision group )
{
	Vector3f Pos(m_WorldMatrix._41, m_WorldMatrix._42, m_WorldMatrix._43);
	Pos += m_pMesh->m_ReglagePivot;
	BoundingBoxLoader Loader;
	if( Loader.Load(physic) == RES_SUCCEED)
	{
		m_ListOfBoundingBox.setPbList( Loader.getvDynamicBody() );
		m_ListOfBoundingBox.setInitialWorldPos( Pos - m_pMesh->m_ReglagePivot );
		
		m_iEmpActor = physX::CreateBoundingBox( m_ListOfBoundingBox, group );
	}
	if(!IsDynamic())
		physX::UpdateObjectFromActor(m_iEmpActor, m_WorldMatrix, m_pMesh->m_ReglagePivot, true);
}

void SceneObject::SetControledCharacter( float radius, float height, void* Ref, GroupCollision group )
{
	Vector3f Pos(m_WorldMatrix._41, m_WorldMatrix._42, m_WorldMatrix._43);
	m_iEmpController = physX::CreateControlledCapsule(Pos, radius, height, Ref, m_iEmpActor, group);
}

void SceneObject::SetControledCharacter(float width, float height, float depth, void* Ref, GroupCollision group )
{
	Vector3f Pos(m_WorldMatrix._41, m_WorldMatrix._42, m_WorldMatrix._43);
	m_iEmpController = physX::CreateControlledBox(Pos, width, height, depth, Ref, m_iEmpActor, group);
}

void SceneObject::SetObjectTrigger( const std::string& physic, Vector3f Pos, void (*OnEnterFunc)(), void (*OnStayFunc)(), void (*OnLeaveFunc)() )
{
	BoundingBoxLoader Loader;
	ListOfBoundingBox BB_List;
	if( Loader.Load(physic) == RES_SUCCEED)
	{
		m_ListOfBoundingBox.setPbList( Loader.getvDynamicBody() );
		m_ListOfBoundingBox.setInitialWorldPos( Pos - m_pMesh->m_ReglagePivot );

		m_iEmpActor = physX::CreateTrigger( m_ListOfBoundingBox, OnEnterFunc, OnLeaveFunc, OnStayFunc);
	}
	if(!IsDynamic())
		physX::getActor(m_iEmpActor)->getGlobalPose().getColumnMajor44( m_WorldMatrix );
}


void SceneObject::SetObjectUnPhysical()
{
	if(IsActor())
		physX::releaseActor( m_iEmpActor );
	else if(IsController())
		physX::releaseController( m_iEmpActor, m_iEmpController );

	m_ListOfBoundingBox.ReleaseList();
}

void SceneObject::SetPhysicalTranslation( float dispX, float dispY, float dispZ )
{
	if(IsActor())
	{
		NxActor* pActor = physX::getActor( m_iEmpActor );
		NxVec3 currentPos = pActor->getGlobalPosition();
		NxVec3 NewPos = currentPos + NxVec3(dispX, dispY, dispZ);
		pActor->setGlobalPosition( NewPos );
	}
	else
	{
		NxController* pControler = physX::getController( m_iEmpController );

		NxU32 collisionFlags; 
		NxF32 minDistance = 0.001f;
		NxVec3 disp( dispX, dispY, dispZ ); 

		pControler->move( disp, MASK_OTHER, minDistance, collisionFlags );
	}
}

void SceneObject::SetPhysicalRotation( float angleX, float angleY, float angleZ )
{
	if(IsActor())
	{
		NxActor* pActor = physX::getActor( m_iEmpActor );

		NxQuat quatCurr = pActor->getGlobalOrientationQuat();
		NxQuat quatX(angleX, NxVec3( 1.f, 0.0f, 0.f));
		NxQuat quatY(angleY, NxVec3( 0.f, 1.0f, 0.f));
		NxQuat quatZ(angleZ, NxVec3( 0.f, 0.0f, 1.f));
		NxQuat quatResult = quatCurr * quatX * quatY * quatZ;

		pActor->setGlobalOrientationQuat(quatResult);
	}
}

void SceneObject::SetPhysicalTransform( const D3DXMATRIX* world )
{
	if(IsActor())
	{
		//Récupération de la matrice du bone et création de celle pour physx
		NxMat33 rotBone(NxVec3( world->_11, world->_12, world->_13),
						NxVec3( world->_21, world->_22, world->_23),
						NxVec3( world->_31, world->_32, world->_33));
		NxVec3 transBone( world->_41, world->_42, world->_43);
		//NxVec3 transBone( world->_14, world->_24, world->_34);

		NxMat34 matBone(rotBone, transBone);
		NxActor* pActor = physX::getActor( m_iEmpActor );
		pActor->setGlobalPose( matBone );
		SetPhysicalRotation(90.f, 0.f, 0.f);
	}
	else
	{
	}
}

//Fonction pour récupérer les infos de la bb pour le culling
std::vector<PhysicBody*> SceneObject::getPhysicBodyList()
{
	return m_ListOfBoundingBox.getPbList();
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

void SceneObject::Init()
{
	InitObject();
}

//===========================================================================//
// Effectue une transformation				                                 //
//===========================================================================//
void SceneObject::SetTransform(const D3DXMATRIX* view, const D3DXMATRIX* proj)
{
	D3DXMATRIX MatWorldView;
	D3DXMATRIX mWorldViewProjection;
	D3DXMATRIX LocalWorld;
	D3DXMatrixIdentity(&LocalWorld);
	LocalWorld._22=0; LocalWorld._23=1;
	LocalWorld._32=1; LocalWorld._33=0;

	D3DXMatrixMultiply(&LocalWorld, &LocalWorld, &m_WorldMatrix);
	D3DXMatrixMultiply(&MatWorldView, &m_WorldMatrix, view);
	D3DXMatrixMultiply(&mWorldViewProjection, &MatWorldView, proj);

	m_pShader->GetEffect()->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection);
	m_pShader->GetEffect()->SetMatrix( "g_mWorld", &m_WorldMatrix);
	m_pShader->GetEffect()->SetMatrix( "g_mView", view);
	D3DXMATRIXA16 mWorldView = (*m_WorldMatrix) * (*view);
	m_pShader->GetEffect()->SetMatrix( "g_mWorldView", &MatWorldView);
}

void SceneObject::SetTransform(const D3DXMATRIX* view, const D3DXMATRIX* proj, const D3DXVECTOR3 CamPos)
{
	SceneObject::SetTransform(view, proj);
	m_pShader->GetEffect()->SetValue("g_vCamPos", CamPos, sizeof(D3DXVECTOR3));
}

void SceneObject::SetTransform(const D3DXMATRIX* world)
{
	if(IsPhysical())
		SetPhysicalTransform( world );
	else
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

void SceneObject::Update()
{
	// Si l'objet est affecté par la physique

		// Maj de l'objet graphique : On utilise la matrice calculée par le moteur physique
		// Si controller multiplié par la matrice de rotation
		// m_WorldMatrix = GetObjectPhysicalMatrix;

	if (IsActor())
	{
		//D3DXMATRIX mat_PhysX;
		//D3DXMatrixIdentity(&mat_PhysX);
		physX::UpdateObjectFromActor( m_iEmpActor, m_WorldMatrix, m_pMesh->m_ReglagePivot );

		//Vector3f  reg =  m_pMesh->m_ReglagePivot;
		//D3DXMatrixTranslation(&m_WorldMatrix, -reg.x, -reg.y, -reg.z);
		//D3DXMatrixMultiply(&m_WorldMatrix, &m_WorldMatrix, &mat_PhysX);

		//physX::UpdateObjectFromActor( m_iEmpActor, m_WorldMatrix );

	}
	else if (IsController())
	{
		SetPhysicalTranslation(0.f, -0.01f, 0.f);
		physX::UpdateObjectFromController( m_iEmpController, m_WorldMatrix, m_pMesh->m_ReglagePivot );
	}
}

void SceneObject::SetTranslation( float x, float y, float z )
{
	// Si l'objet est affecté par la physique
	if(IsPhysical())
		// On affecte la transformation à la physique
		SetPhysicalTranslation(x, y, z);

	// Sinon on transforme l'objet graphique
	else
		Object::SetTranslation(x, y, z);
}

void SceneObject::SetRotation( int angleX, int angleY, int angleZ )
{
	// Si l'objet est affecté par la physique
	if(IsPhysical())
		// On transmet la transformation à la physique
		SetPhysicalRotation((float)angleX, (float)angleY, (float)angleZ);

	// Sinon on transforme l'objet graphique
	else		
		Object::SetRotation(angleX, angleY, angleZ);
}


//===========================================================================//
// Gestion de l'objet						                                 //
//===========================================================================//
void SceneObject::InitDeviceData()
{
	m_pMesh->FillD3DBuffers();
	m_pShader->GetEffect()->OnResetDevice();
}

void SceneObject::Draw()
{
	m_pDevice->SetVertexDeclaration(m_pMesh->m_decl);

	m_pMaterial->SetGraphicalData();

	if(m_bReceiveShadow)
	{
		m_pShader->GetEffect()->SetTechnique( "RenderSceneShadow" );
		m_pShader->GetEffect()->SetTexture("g_TexShadowMap", Renderer::GetInstance()->GetShadowMap()->GetTexShadowMap());
		D3DXMATRIX LightViewProj=m_WorldMatrix*Renderer::GetInstance()->GetShadowMap()->GetLightViewProjMatrix();
		D3DXMATRIX TexProj=LightViewProj*Renderer::GetInstance()->GetShadowMap()->GetTexProjMatrix();
		m_pShader->GetEffect()->SetMatrix("g_mTexProj", &TexProj);
		m_pShader->GetEffect()->SetMatrix("g_mLightViewProj", &LightViewProj);
	}
	else
		m_pShader->GetEffect()->SetTechnique( "RenderScene" );

	m_pShader->GetEffect()->Begin(0, 0);

	m_pShader->GetEffect()->BeginPass(0);

	if (m_pMesh->m_pVB)
	{
			m_pDevice->SetStreamSource(0, m_pMesh->m_pVB, 0, D3DXGetDeclVertexSize(m_pMesh->m_VBdecl, 0));

			m_pDevice->SetIndices ( m_pMesh->m_pIB );

			m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_pMesh->m_iNbVertices, 0, m_pMesh->m_iNbIndex/3); 
	}

	m_pShader->GetEffect()->EndPass();

	m_pShader->GetEffect()->End();

	m_pMaterial->StopGraphicalData();

}

void SceneObject::DrawShadow()
{
	if(!m_bCastShadow)
		return;

	m_pShadowShader=Renderer::GetInstance()->GetShadowMap()->GetDepthShader();

	m_pShadowShader->GetEffect()->SetMatrix( "g_mWorld", &m_WorldMatrix);

	m_pShadowShader->GetEffect()->SetTechnique( "RenderShadow" );

	m_pShadowShader->GetEffect()->Begin(0, 0);

	m_pShadowShader->GetEffect()->BeginPass(0);

	m_pDevice->SetVertexDeclaration(m_pMesh->m_decl);

	if(m_pMesh->m_pVB)
	{
		m_pDevice->SetStreamSource(0, m_pMesh->m_pVB, 0, D3DXGetDeclVertexSize(m_pMesh->m_VBdecl, 0));

		m_pDevice->SetIndices( m_pMesh->m_pIB);

		m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_pMesh->m_iNbVertices, 0, m_pMesh->m_iNbIndex/3);
	}

	m_pShadowShader->GetEffect()->CommitChanges();

	m_pShadowShader->GetEffect()->EndPass();

	m_pShadowShader->GetEffect()->End();
}

void SceneObject::DeleteDeviceData()
{
	m_pMesh->ReleaseD3DBuffers();
	m_pShader->GetEffect()->OnLostDevice();
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


void SceneObject::SetCastShadow(bool value)
{
	m_bCastShadow=value;
}
void SceneObject::SetReceiveShadow(bool value)
{
	m_bReceiveShadow=value;
}


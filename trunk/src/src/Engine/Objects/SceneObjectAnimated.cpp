//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "SceneObjectAnimated.h"
#include "Resources/ResourceManager.h"
#include "Resources/Mesh.h"
#include "Resources/Texture.h"
#include "Resources/Shader.h"
#include "Resources/Anim.h"
#include "Renderer/Renderer.h"
#include "Resources/Material.h"
#include "Renderer/Shadow Map/ShadowMap.h"
#include "Core/System.h"
#include "Core/Time.h"

//===========================================================================//
// Constructeur SceneAnimObject                                              //
//===========================================================================//
SceneObjectAnimated::SceneObjectAnimated()
:SceneObject()
{
}

SceneObjectAnimated::SceneObjectAnimated(const std::string& mesh,
										 const std::string& anim,
										 const D3DXVECTOR3& Position)
										 :SceneObject(mesh, Position)
{
	m_strAnim = anim;
	m_pAnim = NULL;
	m_strShader="default_skinning.fx";
	m_CurrentFrame=0;
	m_bShowBone=false;

	m_AnimPlayer.SetFps(50.f);
}

//===========================================================================//
// Initialisation ressources de l'objet                                      //
//===========================================================================//
void SceneObjectAnimated::InitObject()
{
	m_Instance=Renderer::GetInstance();

	m_pAnim = ResourceManager::GetInstance()->Load<Anim>(m_strAnim);
	SceneObject::InitObject();
	m_pMesh->OrderIndices(m_pAnim->m_BoneArrayOrder);

	if(m_bShowBone)
		m_pBoneMesh=ResourceManager::GetInstance()->Load<Mesh>("showbone.DAE");

	m_matrices=new D3DXMATRIX[m_pAnim->m_Bones.size()];

	m_AnimPlayer.SetNbFrames(m_pAnim->m_NbFrames);
}

void SceneObjectAnimated::Init()
{
	InitObject();
	InitDeviceData();
}

//===========================================================================//
// Initialisation données liées au device                                    //
//===========================================================================//
void SceneObjectAnimated::InitDeviceData()
{
	SceneObject::InitDeviceData();
	if(m_bShowBone)
		m_pBoneMesh->FillD3DBuffers();
}

void SceneObjectAnimated::SetShader(const std::string &strShader)
{
	SceneObject::SetShader(strShader);
}

//===========================================================================//
// Mise à jour de l'animation   		                                     //
//===========================================================================//
void SceneObjectAnimated::UpdateAnimation()
{
	m_AnimPlayer.Update( System::GetInstance()->GetTime()->GetDeltaTime() );

	u32 uCurrentFrame = m_AnimPlayer.GetCurrentFrame();
	if(uCurrentFrame==m_CurrentFrame)
		return;
	m_CurrentFrame = uCurrentFrame;

	std::vector< Bone* >::iterator it=m_pAnim->m_Bones.begin();

	// Pour chaque bones
	while( it != m_pAnim->m_Bones.end() )
	{
		(*it)->FinalMatrix=(*it)->animationMatrix[m_CurrentFrame];

		if((*it)->Parent!=NULL)
			(*it)->FinalMatrix *= (*it)->Parent->FinalMatrix;

		m_matrices[(*it)->Num]=m_pAnim->m_BindShape*(*it)->invBoneSkinMatrix*(*it)->FinalMatrix*m_WorldMatrix;

		++it;
	}
}

//===========================================================================//
// On dessine l'objet					                                     //
//===========================================================================//
void SceneObjectAnimated::Draw()
{
	// Si il ne faut pas afficher
	if(!m_bVisible)
		return;

	std::vector< Bone* >::iterator it=m_pAnim->m_Bones.begin();

	m_pShader->GetEffect()->SetMatrixArray("g_skinningMatrices", m_matrices, (int)m_pAnim->m_Bones.size());

	m_pMaterial->SetGraphicalData();

	m_pDevice->SetVertexDeclaration(m_pMesh->m_decl);

	m_pShader->GetEffect()->SetBool("g_bShowBone", false);

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

}

void SceneObjectAnimated::DrawShadow()
{
	if(!m_bCastShadow)
		return;

	m_pShadowShader=Renderer::GetInstance()->GetShadowMap()->GetDepthShader();

	m_pShadowShader->GetEffect()->SetMatrixArray("g_skinningMatrices", m_matrices, (int)m_pAnim->m_Bones.size());

	m_pShadowShader->GetEffect()->SetTechnique( "RenderShadowAnimated" );

	m_pShadowShader->GetEffect()->Begin(0, 0);

	m_pShadowShader->GetEffect()->BeginPass(0);

	m_pDevice->SetVertexDeclaration(m_pMesh->m_decl);

	if(m_pMesh->m_pVB)
	{
		m_pDevice->SetStreamSource(0, m_pMesh->m_pVB, 0, D3DXGetDeclVertexSize(m_pMesh->m_VBdecl, 0));

		m_pDevice->SetIndices( m_pMesh->m_pIB);

		m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_pMesh->m_iNbVertices, 0, m_pMesh->m_iNbIndex/3);
	}

	m_pShadowShader->GetEffect()->EndPass();

	m_pShadowShader->GetEffect()->End();
}

//===========================================================================//
// Mise à jour matrices de transformation                                    //
//===========================================================================//
void SceneObjectAnimated::SetTransform(const D3DXMATRIX* view, const D3DXMATRIX* proj, const D3DXVECTOR3 CamPos)
{

	D3DXMATRIX MatWorldView;
	D3DXMATRIX mWorldViewProjection;
	D3DXMATRIX mViewProjection;
	

	if(m_bShowBone)
	{
		//D3DXMatrixIdentity(&m_WorldMatrix);
		std::vector< Bone* >::iterator it=m_pAnim->m_Bones.begin();

		// Pour chaque bones
		while( it != m_pAnim->m_Bones.end() )
		{
			D3DXMATRIX boneWorld=(*it)->FinalMatrix*m_WorldMatrix;
			D3DXMatrixMultiply(&MatWorldView, &boneWorld, view);
			D3DXMatrixMultiply(&mWorldViewProjection, &MatWorldView, proj);
			D3DXMatrixMultiply(&mViewProjection, view, proj);

			m_pShader->GetEffect()->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection);

			m_pShader->GetEffect()->SetBool("g_bShowBone", true);

			m_pDevice->SetVertexDeclaration(m_pBoneMesh->m_decl);

			m_pShader->GetEffect()->SetTechnique( "RenderScene" );

			m_pShader->GetEffect()->Begin(0, 0);

			m_pShader->GetEffect()->BeginPass(0);


			if (m_pBoneMesh->m_pVB)
			{
				m_pDevice->SetStreamSource(0, m_pBoneMesh->m_pVB, 0, D3DXGetDeclVertexSize(m_pBoneMesh->m_VBdecl, 0));

				m_pDevice->SetIndices ( m_pBoneMesh->m_pIB );

				m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_pBoneMesh->m_iNbVertices, 0, m_pBoneMesh->m_iNbIndex/3); 
			}

			m_pShader->GetEffect()->EndPass();


			m_pShader->GetEffect()->End();

			++it;
		}
	}

	D3DXMatrixMultiply(&mViewProjection, view, proj);
	m_pShader->GetEffect()->SetMatrix( "g_mViewProj", &mViewProjection);
	m_pShader->GetEffect()->SetValue("g_vCamPos", CamPos, sizeof(D3DXVECTOR3));
}

void SceneObjectAnimated::SetTransform(const D3DXMATRIX* world)
{
	SceneObject::SetTransform(world);
}

//===========================================================================//
// Update de l'objet.                                                        //
//===========================================================================//
void SceneObjectAnimated::Update()
{
	SceneObject::Update();
	
	// Mise à jour animation..
	UpdateAnimation();
}


void SceneObjectAnimated::DeleteData()
{
	SceneObject::DeleteData();
	ResourceManager::GetInstance()->Remove<Anim>(m_strAnim);
	if(m_bShowBone)
		ResourceManager::GetInstance()->Remove<Mesh>("showbone.DAE");

	delete [] m_matrices;
}

void SceneObjectAnimated::SetVisible(bool value)	{m_bVisible = value;}
bool SceneObjectAnimated::GetVisible()				{return m_bVisible;}

//===========================================================================//
// Fonction de contrôle de l'animation                                       //
//===========================================================================//
void SceneObjectAnimated::SetAnimFPS(float fps)
{
	m_AnimPlayer.SetFps(fps);
}

D3DXMATRIX SceneObjectAnimated::GetMatrixTransformBone(int indBone)
{
	D3DXMATRIX BoneTransform=m_pAnim->m_Bones[indBone]->FinalMatrix*m_WorldMatrix;
	return BoneTransform;
}

void SceneObjectAnimated::SetAnim(const std::string& anim)
{
	m_pAnim = ResourceManager::GetInstance()->Load<Anim>(anim);
	m_AnimPlayer.Init();
}



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
	m_bIsRunning=true;
	m_fAnimFPS=50.f;
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
	int uCurrentFrame=(int)(m_Instance->m_fTime*m_fAnimFPS)%m_pAnim->m_NbFrames;
	if(uCurrentFrame<0)
		uCurrentFrame=0;
	if(uCurrentFrame==m_CurrentFrame)
		return;
	m_CurrentFrame=uCurrentFrame;

	std::list< Bone* >::iterator it=m_pAnim->m_Bones.begin();

	// Pour chaque bones
	while( it != m_pAnim->m_Bones.end() )
	{
		(*it)->FinalMatrix=(*it)->animationMatrix[uCurrentFrame];

		if((*it)->Parent!=NULL)
			(*it)->FinalMatrix *= (*it)->Parent->FinalMatrix;

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

	// Mise à jour animation
	if(m_bIsRunning)
		UpdateAnimation();

	std::list< Bone* >::iterator it=m_pAnim->m_Bones.begin();

	// Pour chaque bones
	while( it != m_pAnim->m_Bones.end() )
	{
		m_matrices[(*it)->Num]=m_pAnim->m_BindShape*(*it)->invBoneSkinMatrix*(*it)->FinalMatrix*m_WorldMatrix;
		++it;
	}

	m_pShader->m_pEffect->SetMatrixArray("g_skinningMatrices", m_matrices, (int)m_pAnim->m_Bones.size());

	m_pMaterial->SetGraphicalData();

	m_pDevice->SetVertexDeclaration(m_pMesh->m_decl);

	if(m_bReceiveShadow)
	{
		m_pShader->m_pEffect->SetTechnique( "RenderSceneShadow" );
		m_pShader->m_pEffect->SetTexture("g_TexShadowMap", Renderer::GetInstance()->GetShadowMap()->GetTexShadowMap());
		D3DXMATRIX LightViewProj=m_WorldMatrix*Renderer::GetInstance()->GetShadowMap()->GetLightViewProjMatrix();
		D3DXMATRIX TexProj=LightViewProj*Renderer::GetInstance()->GetShadowMap()->GetTexProjMatrix();
		m_pShader->m_pEffect->SetMatrix("g_mTexProj", &TexProj);
		m_pShader->m_pEffect->SetMatrix("g_mLightViewProj", &LightViewProj);
	}
	else
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

void SceneObjectAnimated::DrawShadow()
{
	if(!m_bCastShadow)
		return;

	m_pShadowShader=Renderer::GetInstance()->GetShadowMap()->GetDepthShader();

	m_pShadowShader->m_pEffect->SetMatrixArray("g_skinningMatrices", m_matrices, (int)m_pAnim->m_Bones.size());

	m_pShadowShader->m_pEffect->SetTechnique( "RenderShadowAnimated" );

	m_pShadowShader->m_pEffect->Begin(0, 0);

	m_pShadowShader->m_pEffect->BeginPass(0);

	m_pDevice->SetVertexDeclaration(m_pMesh->m_decl);

	if(m_pMesh->m_pVB)
	{
		m_pDevice->SetStreamSource(0, m_pMesh->m_pVB, 0, D3DXGetDeclVertexSize(m_pMesh->m_VBdecl, 0));

		m_pDevice->SetIndices( m_pMesh->m_pIB);

		m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_pMesh->m_iNbVertices, 0, m_pMesh->m_iNbIndex/3);
	}

	m_pShadowShader->m_pEffect->EndPass();

	m_pShadowShader->m_pEffect->End();
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
		std::list< Bone* >::iterator it=m_pAnim->m_Bones.begin();

		// Pour chaque bones
		while( it != m_pAnim->m_Bones.end() )
		{

			// Matrice de départ du bone
			D3DXMATRIX boneWorld=m_pAnim->m_BindShape*(*it)->FinalMatrix*m_WorldMatrix;
			D3DXMatrixMultiply(&MatWorldView, &boneWorld, view);
			D3DXMatrixMultiply(&mWorldViewProjection, &MatWorldView, proj);
			D3DXMatrixMultiply(&mViewProjection, view, proj);

			m_pShader->m_pEffect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection);

			m_pShader->m_pEffect->SetBool("g_bShowBone", true);

			m_pDevice->SetVertexDeclaration(m_pBoneMesh->m_decl);

			m_pShader->m_pEffect->SetTechnique( "RenderScene" );

			m_pShader->m_pEffect->Begin(0, 0);

			m_pShader->m_pEffect->BeginPass(0);


			if (m_pBoneMesh->m_pVB)
			{
				m_pDevice->SetStreamSource(0, m_pBoneMesh->m_pVB, 0, D3DXGetDeclVertexSize(m_pBoneMesh->m_VBdecl, 0));

				m_pDevice->SetIndices ( m_pBoneMesh->m_pIB );

				m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_pBoneMesh->m_iNbVertices, 0, m_pBoneMesh->m_iNbIndex/3); 
			}

			m_pShader->m_pEffect->EndPass();


			m_pShader->m_pEffect->End();

			++it;
		}
	}

	D3DXMatrixMultiply(&mViewProjection, view, proj);
	m_pShader->m_pEffect->SetMatrix( "g_mViewProj", &mViewProjection);
	m_pShader->m_pEffect->SetValue("g_vCamPos", CamPos, sizeof(D3DXVECTOR3));
}

void SceneObjectAnimated::SetTransform(const D3DXMATRIX* world)
{
	SceneObject::SetTransform(world);
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

void SceneObjectAnimated::StartAnim()
{
	m_bIsRunning=true;
}

void SceneObjectAnimated::PauseAnim()
{
	m_bIsRunning=!m_bIsRunning;
}

void SceneObjectAnimated::StopAnim()
{
	std::list< Bone* >::iterator it=m_pAnim->m_Bones.begin();

	// Pour chaque bones
	while( it != m_pAnim->m_Bones.end() )
	{
		(*it)->FinalMatrix=(*it)->animationMatrix[0];

		if((*it)->Parent!=NULL)
			(*it)->FinalMatrix *= (*it)->Parent->FinalMatrix;

		++it;

	}
	m_bIsRunning=false;

}

void SceneObjectAnimated::SetAnimFPS(float fps)
{
	m_fAnimFPS=fps;
}

void SceneObjectAnimated::SetAnim(const std::string& anim)
{
	m_pAnim = ResourceManager::GetInstance()->Load<Anim>(anim);
}



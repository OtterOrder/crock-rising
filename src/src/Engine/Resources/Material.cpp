//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "Material.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "Texture.h"
#include "Objects/Light.h"

Materials::Materials()
{
	m_DiffuseColor=D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.f);
	m_AmbientColor=D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.f);
	m_SpecularColor=D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
	m_Maps[TEX_MESH]=NULL;
	m_Opacity=1.f;
	m_Glossiness=30.f;
	m_pShader=NULL;
	m_LightList=&Light::RefList;
}

void Materials::SetShader(Shader* shader)
{
	m_pShader=shader;
}

void Materials::SetTexture(const std::string& strTex, TextureType Type)
{
	switch(Type)
	{
	case TEX_MESH:
		m_Maps[TEX_MESH]=ResourceManager::GetInstance()->Load<Texture>(strTex);
		break;
	}

}

void Materials::SetTexture(LPDIRECT3DTEXTURE9 Tex, TextureType Type)
{
	switch(Type)
	{
	case TEX_MESH:
		m_Maps[TEX_MESH]=new Texture();
		m_Maps[TEX_MESH]->m_pTex=Tex;
		break;
	}

}


void Materials::SetGraphicalData()
{
	// On transmet les textures de l'objet au shader
	if(m_Maps[TEX_MESH])
	{
		m_pShader->m_pEffect->SetBool("g_UseTex", true);
		m_pShader->m_pEffect->SetTexture("g_MeshTexture", m_Maps[TEX_MESH]->m_pTex);
	}

	// On transmet les couleurs de l'objet au shader
	m_pShader->m_pEffect->SetVector("g_ObjectDiffuse", &m_DiffuseColor);
	m_pShader->m_pEffect->SetVector("g_ObjectAmbient", &m_AmbientColor);
	m_pShader->m_pEffect->SetVector("g_ObjectSpecular", &m_SpecularColor);
	m_pShader->m_pEffect->SetValue("g_Glossiness", (void*)&m_Glossiness, sizeof(float));

	// On transmet les lumières au shader
	std::list< Light* >::iterator it=m_LightList->begin();
	while(it != m_LightList->end())
	{
		(*it)->SetLightData(m_pShader);
		++it;
	}

}

void Materials::SetTextureData()
{
	if(m_Maps[TEX_MESH])
	{
		m_pShader->m_pEffect->SetBool("g_UseTex", true);
		m_pShader->m_pEffect->SetTexture("g_MeshTexture", m_Maps[TEX_MESH]->m_pTex);
	}

}

void Materials::DeleteData()
{
	for(std::map<TextureType, Texture*>::const_iterator it=m_Maps.begin() ; it!=m_Maps.end() ; ++it)
	{
		if((it->second)!=NULL)
			ResourceManager::GetInstance()->Remove<Texture>(it->second->GetName());
	}
}
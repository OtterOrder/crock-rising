//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "Material.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "Texture.h"
#include "Objects/Light.h"
#include "Renderer/Renderer.h"

Material::Material()
{
	m_DiffuseColor=D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.f);
	m_AmbientColor=D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.f);
	m_SpecularColor=D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
	m_Maps[Texture::DIFFUSE]=NULL;
	m_Opacity=1.f;
	m_Glossiness=30.f;
	m_pShader=NULL;
	m_LightList=&Light::RefList;
	m_pDevice=Renderer::GetInstance()->m_pd3dDevice;
}

void Material::SetShader(Shader* shader)
{
	m_pShader=shader;
}

void Material::SetTexture(const std::string& strTex, Texture::Type Type)
{
	switch(Type)
	{
		case Texture::DIFFUSE:
			m_Maps[Texture::DIFFUSE]=ResourceManager::GetInstance()->Load<Texture>(strTex);
			break;

		case Texture::NORMALMAP:
			m_Maps[Texture::NORMALMAP]=ResourceManager::GetInstance()->Load<Texture>(strTex);
			break;
		case Texture::ALPHAMAP:
			m_Maps[Texture::ALPHAMAP]=ResourceManager::GetInstance()->Load<Texture>(strTex);
			break;
	}

}

void Material::SetTexture(LPDIRECT3DTEXTURE9 Tex, Texture::Type Type)
{
	switch(Type)
	{
		case Texture::DIFFUSE:
			m_Maps[Texture::DIFFUSE]=new Texture();
			m_Maps[Texture::DIFFUSE]->SetTexture( Tex );
			break;

		case Texture::NORMALMAP:
			m_Maps[Texture::NORMALMAP]=new Texture();
			m_Maps[Texture::NORMALMAP]->SetTexture( Tex );
			break;
	}

}


void Material::SetGraphicalData()
{
	// On transmet les textures de l'objet au shader
	if(m_Maps[Texture::DIFFUSE])
	{
		m_pShader->GetEffect()->SetBool("g_UseTex", true);
		m_pShader->GetEffect()->SetTexture("g_MeshTexture", m_Maps[Texture::DIFFUSE]->GetTexture());

		m_pShader->GetEffect()->SetBool("g_UseNormalMap", false);
		if (m_Maps[Texture::NORMALMAP])
		{
			m_pShader->GetEffect()->SetBool("g_UseNormalMap", true);
			m_pShader->GetEffect()->SetTexture("g_NormalMapTexture", m_Maps[Texture::NORMALMAP]->GetTexture());
		}
	}
	else
		m_pShader->GetEffect()->SetBool("g_UseTex", false);

	// On transmet les couleurs de l'objet au shader
	m_pShader->GetEffect()->SetVector("g_ObjectDiffuse", &m_DiffuseColor);
	m_pShader->GetEffect()->SetVector("g_ObjectAmbient", &m_AmbientColor);
	m_pShader->GetEffect()->SetVector("g_ObjectSpecular", &m_SpecularColor);
	m_pShader->GetEffect()->SetValue("g_Glossiness", (void*)&m_Glossiness, sizeof(float));

	// On transmet les lumières au shader
	std::list< Light* >::iterator it=m_LightList->begin();
	while(it != m_LightList->end())
	{
		(*it)->SetLightData(m_pShader);
		++it;
	}

	if(m_Opacity<1.f)
	{
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		m_pShader->GetEffect()->SetValue("g_Opacity", (void*)&m_Opacity, sizeof(float));
	}

}

void Material::StopGraphicalData()
{
	if(m_Opacity<1.f)
	{
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}
}

void Material::SetTextureData()
{
	if(m_Maps[Texture::DIFFUSE])
	{
		m_pShader->GetEffect()->SetBool("g_UseTex", true);
		m_pShader->GetEffect()->SetTexture("g_MeshTexture", m_Maps[Texture::DIFFUSE]->GetTexture());
	}

}

void Material::DeleteData()
{
	for(std::map<Texture::Type, Texture*>::const_iterator it=m_Maps.begin() ; it!=m_Maps.end() ; ++it)
	{
		if((it->second)!=NULL)
			ResourceManager::GetInstance()->Remove<Texture>(it->second->GetName());
	}
}
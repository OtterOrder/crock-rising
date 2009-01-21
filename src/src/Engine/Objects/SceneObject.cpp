#include "SceneObject.h"

//===========================================================================//
// Constructeurs SceneObject		                                         //
//===========================================================================//
SceneObject::SceneObject()
{
}

SceneObject::SceneObject(const std::string& mesh, const std::string& Tex, const D3DXVECTOR3& Position)
{
	m_Mesh=mesh;
	m_MapTexture[MESHTEX]=Tex;
}

//===========================================================================//
// Ajoute une texture à la map		                                         //
//===========================================================================//
void SceneObject::SetTexture(const std::string& Tex, types_tex Type)
{
	// On teste si la resource existe déjà
	Texture * tex=ResourceManager::GetInstance()->Load<Texture>(Tex);
	
	m_MapTexture[Type]=Tex;

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
	Shader * shader=ResourceManager::GetInstance()->Load<Shader>(Shad);
}

void SceneObject::InitObject()
{
	m_PtrMesh=ResourceManager::GetInstance()->Load<Mesh>(m_Mesh);
	m_PtrTex=ResourceManager::GetInstance()->Load<Texture>(m_MapTexture[MESHTEX]);
}

//===========================================================================//
// Effectue une transformation				                                 //
//===========================================================================//
void SceneObject::SetTransform(const D3DXMATRIX *world)
{

}

void SceneObject::SetTransform(const D3DXMATRIX* world, const D3DXMATRIX* view, const D3DXMATRIX* proj)
{

}

void SceneObject::SetTransform(const D3DXMATRIX* world, const D3DXMATRIX* view, const D3DXMATRIX* proj, const D3DXVECTOR3 CamPos)
{

}

//===========================================================================//
// Gestion de l'objet						                                 //
//===========================================================================//
void SceneObject::InitDeviceData()
{

}

void SceneObject::FrameMove(float fElapsedTime)
{

}

void SceneObject::Draw()
{

}

void SceneObject::DeleteDeviceData()
{

}

void SceneObject::DeleteData()
{

}

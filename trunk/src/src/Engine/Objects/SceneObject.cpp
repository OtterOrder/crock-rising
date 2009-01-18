#include "SceneObject.h"

//===========================================================================//
// Constructeurs SceneObject		                                         //
//===========================================================================//
SceneObject::SceneObject()
{
}

SceneObject::SceneObject(const crc32& mesh, const crc32& Tex, const D3DXVECTOR3& Position)
{
	m_Mesh=mesh;
	m_MapTexture[MESHTEX]=Tex;
}

//===========================================================================//
// Ajoute une texture à la map		                                         //
//===========================================================================//
void SceneObject::SetTexture(const crc32& Tex, types_tex Type)
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
void SceneObject::SetShader(const crc32& Shad)
{
	Shader * shader=ResourceManager::GetInstance()->Load<Shader>(Shad);
}

void SceneObject::InitObject()
{

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

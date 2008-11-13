#include "SceneObject.h"

// Constructeur
SceneObject::SceneObject()
{
}

SceneObject::SceneObject( crc32 Mesh, crc32 Texture, Vector3f Position, Vector3f Rotation, Vector3f Scale)
					:	m_Mesh(Mesh), m_Rotation(Rotation), m_Scale(Scale)
{
	m_Position = Position;
	m_Texture.push_back( Texture );
}

// Ajout le crc d'une texture au tableau
void SceneObject::AddTexture(crc32 CrcTex )
{
	m_Texture.push_back( CrcTex );
}


// Vide le tableau des crc texture
void SceneObject::ReleaseTexture()
{
	m_Texture.clear();
}

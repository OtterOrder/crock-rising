#ifndef		_SceneObject_H
#define		_SceneObject_H

//******************************************************************

#include	<libCore/Types/Crc32.h>
#include	"Object.h"
#include	<vector>

using namespace std;

//******************************************************************

class SceneObject : public Object
{
public:
	SceneObject();
	SceneObject( crc32 Mesh, crc32 Texture, Vector3f Position, Vector3f Rotation, Vector3f Scale);

protected:
	// Rotation d'un objet de scene
	void			SetRotation( Vector3f rotation ){	m_Rotation = rotation;	}
	Vector3f		GetRotation()					{	return m_Rotation;		}

	// Changement d'echelle d'un objet de scene
	void			SetScale( Vector3f scale )		{	m_Scale = scale;		}
	Vector3f		GetScale()						{	return m_Scale;			}

	// Gestion des textures des objets
	unsigned int	GetNbTexture()					{	return unsigned int(m_Texture.size());	}
	void			AddTexture( crc32 CrcTex );		// Ajout d'une texture au tableau
	void			ReleaseTexture();				// Vide le tableau des textures
	
private:
	Vector3f m_Rotation;
	Vector3f m_Scale;

	unsigned int	m_uNbTexture;
	vector<crc32>	m_Texture;

	crc32 m_Mesh;
};

//******************************************************************
#endif		// _SceneObject_H

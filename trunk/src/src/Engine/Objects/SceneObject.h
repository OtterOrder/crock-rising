#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "Core/Types/Crc32.h"
#include "Object.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/Texture.h"
#include "../Resources/Shader.h"
#include "../Resources/Mesh.h"
#include <map>

enum types_tex
{
	MESHTEX,
	NORMALMAP,
	BUMPMAP,
	DISPLACEMENTMAP
};

//===========================================================================//
// Classe pour un objet affichable dans la sc�ne 3D                          //
//===========================================================================//
class SceneObject : public Object
{
public:
	SceneObject(const std::string& mesh, const std::string& Tex, const D3DXVECTOR3& Position);
	SceneObject();
	virtual ~SceneObject();

	//===========================================================================//
	// Gestion apparence des objets											     //
	//===========================================================================//
	void	SetTexture(const std::string& Tex, types_tex Type);	// Ajout d'une texture
	void	SetShader(const std::string& Shad); 

protected:

	//===========================================================================//
	// Utilisation de l'objet													 //
	//===========================================================================//
	void	InitObject();
	void	SetTransform(const D3DXMATRIX* world, const D3DXMATRIX* view, const D3DXMATRIX* proj);
	void	SetTransform(const D3DXMATRIX* world, const D3DXMATRIX* view, const D3DXMATRIX* proj, const D3DXVECTOR3 CamPos);
	void	SetTransform(const D3DXMATRIX* world);
	void	InitDeviceData();
	void	FrameMove(float fElapsedTime);
	void	Draw();
	void	DeleteDeviceData();
	void	DeleteData();
	
private:

	//===========================================================================//
	// Types												                     //
	//===========================================================================//
	typedef std::map<types_tex, std::string> TTextureMap;

	//===========================================================================//
	// Donn�es membres												             //
	//===========================================================================//
	std::string		m_Mesh;
	std::string		m_Shader;
	TTextureMap	m_MapTexture;

	Mesh * m_PtrMesh;
	Texture * m_PtrTex;
	Shader * m_PtrShader;
	
};


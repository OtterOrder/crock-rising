#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "Core/Types/Crc32.h"
#include "Object.h"
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
// Classe pour un objet affichable dans la scène 3D                          //
//===========================================================================//
class SceneObject : public Object
{
public:
	SceneObject(const std::string& mesh, const std::string& Tex, const D3DXVECTOR3& Position, const std::string shader = "default.fx");
	SceneObject();
	virtual ~SceneObject() {}

	//===========================================================================//
	// Gestion apparence des objets											     //
	//===========================================================================//
	void	SetTexture(const std::string& Tex, types_tex Type);	// Ajout d'une texture
	void	SetShader(const std::string& Shad); 

public:

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
	typedef std::map<types_tex, Texture*> TTextureMap;

	//===========================================================================//
	// Données membres												             //
	//===========================================================================//
	std::string		m_Mesh;
	std::string		m_Tex;
	std::string		m_Shader;
	TTextureMap		m_MapTexture;

	
	Shader*			m_PtrShader;
	D3DXVECTOR3		m_Offset;

	Mesh*			m_PtrMesh;
	
};


#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include <map>
#include <list>
#include "Texture.h"
#include <d3dx9.h>

class Shader;
class Texture;
class Light;

class Material
{
	friend class SceneObject;
	friend class SceneObjectAnimated;

public:
	Material();
	~Material(){}

	//===========================================================================//
	// Propriétés de l'objet    							                     //
	//===========================================================================//
	D3DXVECTOR4		m_DiffuseColor;
	D3DXVECTOR4		m_AmbientColor;
	D3DXVECTOR4		m_SpecularColor;
	float			m_Opacity;
	float			m_Glossiness;

private:
	//===========================================================================//
	// Types												                     //
	//===========================================================================//
	typedef std::map<Texture::Type, Texture*> TTextureMap;

public:
	void			SetTexture(const std::string& strTex, Texture::Type Type);
	void			SetTexture(LPDIRECT3DTEXTURE9 Tex, Texture::Type Type);

protected:
	void			SetShader(Shader* shader);
	void			SetGraphicalData();
	void			SetTextureData();
	void			DeleteData();

	Shader*					m_pShader;
	TTextureMap				m_Maps;
	std::list< Light* >*	m_LightList; // Liste des lumières de la scène

};
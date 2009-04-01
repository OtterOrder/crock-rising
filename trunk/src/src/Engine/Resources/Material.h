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

class Materials
{
	friend class SceneObject;
	friend class SceneObjectAnimated;

public:
	Materials();
	~Materials(){}

	//===========================================================================//
	// Propri�t�s de l'objet    							                     //
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
	typedef std::map<TextureType, Texture*> TTextureMap;

public:
	void			SetTexture(const std::string& strTex, TextureType Type);
	void			SetTexture(LPDIRECT3DTEXTURE9 Tex, TextureType Type);

protected:
	void			SetShader(Shader* shader);
	void			SetGraphicalData();
	void			SetTextureData();
	void			DeleteData();

	Shader*					m_pShader;
	TTextureMap				m_Maps;
	std::list< Light* >*	m_LightList; // Liste des lumi�res de la sc�ne

};
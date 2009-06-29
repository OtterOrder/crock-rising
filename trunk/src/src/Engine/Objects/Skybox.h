#pragma once

#include "Object.h"
#include "Resources/Shader.h"

class Skybox : public Object
{
public:
	Skybox(const std::string& Name);
	Skybox();
	~Skybox();

	HRESULT Init(float _size=10);
	HRESULT ResetDevice();
	void	SetTransform(const D3DXMATRIX* view, const D3DXMATRIX* proj, const D3DXVECTOR3 CamPos);
	HRESULT Draw();
	HRESULT LostDevice();
	void	DeleteData();

protected:
	std::string					m_StrTex;
	LPDIRECT3DVERTEXBUFFER9 	m_pSkyVB;
	IDirect3DCubeTexture9*		m_pEnvTex; 
	Shader*						m_pSkyShader;
	float							m_size;


};
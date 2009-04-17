#pragma once


//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include <Renderer/Renderer.h>
#include <Objects/Light.h>

#define SHADOW_MAP_SIZE 512

class ShadowMap : public Singleton< ShadowMap >
{
public:
	ShadowMap();
	~ShadowMap();

	void SetShadowMap(Light * ShadowLight);

	void Release();

	

protected:
	void InitShadowMap();

	LPDIRECT3DTEXTURE9	m_pShadowMap;
	LPDIRECT3DSURFACE9	m_pShadowSurf;

	LPDIRECT3DDEVICE9   m_pDevice;

	std::list< SceneObject* >	*m_ScObjList;			// Liste des objets de sc�ne
	Light						*m_ShadowLight;			// Lumi�re qui ombre la sc�ne
	D3DXMATRIX					 m_mLightViewProj;      // Matrice de la lumi�re
};
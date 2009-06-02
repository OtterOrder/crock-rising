#pragma once


//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include <Renderer/Renderer.h>
#include <Objects/Light.h>
#include <Resources/Shader.h>

#define SHADOW_MAP_SIZE 512

class ShadowMap : public Singleton< ShadowMap >
{
public:
	ShadowMap();
	~ShadowMap();

	void SetShadowMap(Light * ShadowLight);

	void Release();
	void ResetDevice();
	void LostDevice();
	void Render();
	LPDIRECT3DTEXTURE9 GetTexShadowMap(){return m_pShadowMap;}
	D3DXMATRIX		   GetTexProjMatrix(){return m_mTexProj;}
	D3DXMATRIX		   GetLightWorldViewProjMatrix(){return m_mLightWorldViewProj;}
	D3DXMATRIX		   GetLightViewProjMatrix(){return m_mLightViewProj;}
	Shader*			   GetDepthShader(){return m_pShadowShader;}


protected:
	void InitShadowMap();

	LPDIRECT3DSURFACE9	m_pShadowSurf;
	LPDIRECT3DTEXTURE9	m_pShadowMap;					// La shadow map qui sera utilis�e dans le moteur
	LPDIRECT3DSURFACE9	m_pOldColorRT;

	LPDIRECT3DSURFACE9	m_pShadowDepth;
	LPDIRECT3DSURFACE9	m_pOldDepthRT;


	LPDIRECT3DDEVICE9   m_pDevice;

	std::list< SceneObject* >	*m_ScObjList;			// Liste des objets de sc�ne
	Light						*m_ShadowLight;			// Lumi�re qui ombre la sc�ne
	D3DXMATRIX					 m_mLightProj;
	D3DXMATRIX					 m_mLightView;
	D3DXMATRIX					 m_mLightWorldViewProj;      
	D3DXMATRIX					 m_mLightViewProj;      // Matrice de la lumi�re
	D3DXMATRIX					 m_mTexProj;
	Shader						*m_pShadowShader;		// Shader pour rendre la shadow map

};
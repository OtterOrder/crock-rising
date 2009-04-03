#pragma once

#include	"Core/Types/Types.h"
#include	"Core/Singleton.h"
#include	<D3D9.h>
#include	<D3Dx9.h>

#include	"RenderTarget.h"
#include	"PostEffect.h"
#include	"MotionBlur.h"

#include	<vector>

//----------------------------------------------------------------------------------------------
class PostRenderer: public Singleton< PostRenderer >
{
public:
	PostRenderer(void);
	virtual ~PostRenderer(void);

protected:
	LPDIRECT3DSURFACE9	m_pBackBuffer;

	RenderTarget*		m_pSceneRenderTarget;

	std::vector <PostEffect*> m_pPostEffects;
public:
	void RenderPostEffects ();

	void SetBackBuffer (LPDIRECT3DSURFACE9 _pBackBuffer);

	//void SetSceneRenderTexture (LPDIRECT3DTEXTURE9 _pSceneRenderTexture);

	LPDIRECT3DTEXTURE9 GetSceneRenderTexture ()		{ return m_pSceneRenderTarget->GetTexture(); };
	LPDIRECT3DSURFACE9 GetSceneRenderSurface ()		{ return m_pSceneRenderTarget->GetSurface(); };

	void ReleaseSceneRender ();
	HRESULT CreateSceneRender (LPDIRECT3DDEVICE9 _pDevice, u32 _width, u32 _height);
};
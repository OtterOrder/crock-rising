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
	typedef enum
	{
		PE_MotionBlur =0
	} PostEffectType;

public:
	PostRenderer(void);
	virtual ~PostRenderer(void);

protected:
	u8	m_PostEffects;

	LPDIRECT3DSURFACE9	m_pBackBuffer;
	LPDIRECT3DDEVICE9   m_pDevice;

	RenderTarget*		m_pSceneRenderTarget;
	RenderTarget*		m_pSceneRenderTargetTemp;

	std::vector <PostEffect*> m_pPostEffects;
public:
	void RenderPostEffects ();

	void SetBackBuffer (LPDIRECT3DSURFACE9 _pBackBuffer);
	LPDIRECT3DSURFACE9 GetBackBuffer () { return m_pBackBuffer; };

	LPDIRECT3DTEXTURE9 GetSceneRenderTexture ()		{ return m_pSceneRenderTarget->GetTexture(); };
	LPDIRECT3DSURFACE9 GetSceneRenderSurface ()		{ return m_pSceneRenderTarget->GetSurface(); };

	LPDIRECT3DTEXTURE9 GetSceneRenderTextureTemp ()		{ return m_pSceneRenderTargetTemp->GetTexture(); };
	LPDIRECT3DSURFACE9 GetSceneRenderSurfaceTemp ()		{ return m_pSceneRenderTargetTemp->GetSurface(); };


	HRESULT Create (LPDIRECT3DDEVICE9 _pDevice, u32 _width, u32 _height);
	void Release ();
	void Destroy ();

	void SwapSceneRender ();

	void SetRenderTarget (LPDIRECT3DSURFACE9 _pRenderTarget);
	LPDIRECT3DSURFACE9 GetRenderTarget ();

	void SetRenderTarget (u32 _level, LPDIRECT3DSURFACE9 _pRenderTarget);
	LPDIRECT3DSURFACE9 GetRenderTarget (u32 _level);

	void EnablePostEffect	(PostEffectType _postEffect)	{ m_PostEffects = m_PostEffects | (1 << _postEffect); };
	void DisablePostEffect	(PostEffectType _postEffect)	{ m_PostEffects = !(!m_PostEffects | (1 << _postEffect)); };
};
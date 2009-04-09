#pragma once

#include	"Core/Types/Vector.h"
#include	<D3D9.h>
#include	<D3Dx9.h>

class RenderTarget
{
	u32		m_width;
	u32		m_height;

	LPDIRECT3DTEXTURE9	m_pTexture;
	LPDIRECT3DSURFACE9	m_pSurface;

public:
	RenderTarget(void);
	RenderTarget(u32 _width, u32 _height);
	virtual ~RenderTarget(void);

	void Release ();
	HRESULT Create (LPDIRECT3DDEVICE9 _pDevice);

	LPDIRECT3DTEXTURE9 GetTexture ()	{ return m_pTexture; };
	LPDIRECT3DSURFACE9 GetSurface ()	{ return m_pSurface; };

	void operator = (const RenderTarget& _RenderTarget);
};
#pragma once

#include	"Core/Types/Vector.h"
#include	<D3D9.h>
#include	<D3Dx9.h>

class RenderTarget
{
	Coord m_Size;

	LPDIRECT3DTEXTURE9	m_pTexture;
	LPDIRECT3DSURFACE9	m_pSurface;

public:
	RenderTarget(void);
	RenderTarget(Coord _Size);
	virtual ~RenderTarget(void);

	void Release ();

	HRESULT Create (LPDIRECT3DDEVICE9 _pDevice);

	LPDIRECT3DTEXTURE9 GetTexture ()	{ return m_pTexture; };
	LPDIRECT3DSURFACE9 GetSurface ()	{ return m_pSurface; };
};

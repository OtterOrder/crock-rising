#pragma once

#include	"PostEffect.h"

#include	<D3D9.h>
#include	"Objects/2D/Quad.h"

#include	"PostPorcesses/PostRenderer.h"

class VectorialBlur : public PostEffect, public Singleton< VectorialBlur >
{
public:
	LPDIRECT3DTEXTURE9	m_pTextureToBlur;
	LPDIRECT3DTEXTURE9	m_pVectorsTexture;
	LPDIRECT3DSURFACE9	m_pBlurredSurface;

protected:
	Quad*	m_pRenderQuad;

	LPDIRECT3DSURFACE9	m_pCurrentRenderTarget;

public:
	VectorialBlur(void);
	virtual ~VectorialBlur(void);

	virtual void Create (LPDIRECT3DDEVICE9 _pDevice =NULL, u32 _width =0, u32 _height =0);
	virtual void Release ();
	virtual void Destroy ();

	virtual void Apply(list<SceneObject*>* m_pObjectList = NULL);
};
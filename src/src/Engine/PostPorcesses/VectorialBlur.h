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
	LPDIRECT3DSURFACE9	m_pSurfaceToBlur;
	Quad*	m_pRenderQuad;

	LPDIRECT3DSURFACE9	m_pCurrentRenderTarget;

public:
	VectorialBlur(void);
	virtual ~VectorialBlur(void);

	virtual void Apply(list<SceneObject*>* m_pObjectList = NULL);
};
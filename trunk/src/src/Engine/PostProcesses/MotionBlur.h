#pragma once

#include	"PostEffect.h"
#include	"Resources/Shader.h"

#include	"PostProcesses/PostRenderer.h"
#include	"PostProcesses/VectorialBlur.h"

//////////////////////////////////////////////////////////////////////////
// Todo :	- Skinning Velocity
//			- Update Size & Device
//			- Bidirectional blur
//////////////////////////////////////////////////////////////////////////

class MotionBlur : public PostEffect
{
public:
	MotionBlur(void);
	virtual ~MotionBlur(void);

	virtual void Create (LPDIRECT3DDEVICE9 _pDevice, u32 _width, u32 _height);
	virtual void Release ();
	virtual void Destroy ();

	virtual void Apply(list<SceneObject*>* _pObjectList = NULL);

protected:
	RenderTarget*	m_pVelocity;

	D3DXMATRIX m_ViewProj;
	D3DXMATRIX m_LastViewProj;

	void RenderVelocity (list<SceneObject*>* _pObjectList);
};
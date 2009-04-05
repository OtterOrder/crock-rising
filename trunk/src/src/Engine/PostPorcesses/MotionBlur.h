#pragma once

#include	"PostEffect.h"
#include	"Resources/Shader.h"

#include	"PostPorcesses/PostRenderer.h"
#include	"PostPorcesses/VectorialBlur.h"

//////////////////////////////////////////////////////////////////////////
// Todo :	- Skinning Velocity
//			- Update Size & Device
//			- Bidirectional blur
//////////////////////////////////////////////////////////////////////////

class MotionBlur : public PostEffect
{
public:
	MotionBlur(void);
	MotionBlur(u32 _width, u32 _height);
	virtual ~MotionBlur(void);

	virtual void Apply(list<SceneObject*>* _pObjectList = NULL);

protected:
	RenderTarget*	m_pVelocity;

	D3DXMATRIX m_ViewProj;
	D3DXMATRIX m_LastViewProj;

	void RenderVelocity (list<SceneObject*>* _pObjectList);
};
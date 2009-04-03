#pragma once

#include	"PostEffect.h"
#include	"Resources/Shader.h"

class MotionBlur : public PostEffect
{
public:
	MotionBlur(void);
	virtual ~MotionBlur(void);

	virtual void Apply(list<SceneObject*>* m_pObjectList = NULL);
};

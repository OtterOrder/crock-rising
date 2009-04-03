#pragma once

#include	<string>
#include	<list>
#include	"Resources/Shader.h"
#include	"Renderer/Renderer.h"
#include	"Objects/SceneObject.h"

using namespace std;

class PostEffect
{
	Shader*	m_pShader;
public:
	PostEffect(void);
	virtual ~PostEffect(void);

	virtual void Apply(list<SceneObject*>* m_pObjectList = NULL) = 0;
protected:
	void SetShader (string _shaderFileName);
};

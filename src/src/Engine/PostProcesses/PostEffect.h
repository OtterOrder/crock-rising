#pragma once

#include	<string>
#include	<list>

#include	"Resources/Shader.h"
#include	"Renderer/Renderer.h"
#include	"Objects/SceneObject.h"

#include	"RenderTarget.h"

using namespace std;

class PostEffect
{
protected:
	Shader*	m_pShader;

public:
	PostEffect(void);
	virtual ~PostEffect(void);

	virtual void Create (LPDIRECT3DDEVICE9 _pDevice, u32 _width, u32 _height) =0;
	virtual void Release () =0;
	virtual void Destroy () =0;

	virtual void Apply(list<SceneObject*>* m_pObjectList = NULL) = 0;
protected:
	void SetShader (string _shaderFileName);
};

#include "MotionBlur.h"

#include "Resources/Mesh.h"

MotionBlur::MotionBlur(void)
{
	SetShader("motionBlurVelocity.fx");
}

MotionBlur::~MotionBlur(void)
{
}

void MotionBlur::Apply(list<SceneObject*>* m_pObjectList)
{
	if (!m_pObjectList)
		return;

	Mesh* pMesh = NULL;

	D3DXMATRIX ViewProj = Renderer::GetInstance()->GetViewProjMatrix();

	SceneObject::ScObjIt scobj;
	scobj = m_pObjectList->begin();
	while( scobj != m_pObjectList->end() )
	{
		/*
		#ifdef DEVCAMERA
		(*scobj)->SetTransform(m_DevCamera.GetViewMatrix(), m_DevCamera.GetProjMatrix(), *m_DevCamera.GetEyePt());
		#else
		(*scobj)->SetTransform(&m_Camera->GetViewMatrix(), &m_Camera->GetProjMatrix());
		#endif
		(*scobj)->Draw();
		++scobj;
		*/
		pMesh = (*scobj)->GetMesh();
	}
}

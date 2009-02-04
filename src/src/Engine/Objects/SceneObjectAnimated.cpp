#include "SceneObjectAnimated.h"
#include "Resources/ResourceManager.h"
#include "Resources/Anim.h"

SceneObjectAnimated::SceneObjectAnimated()
:SceneObject()
{
}

SceneObjectAnimated::SceneObjectAnimated(const std::string& mesh,
										 const std::string& anim,
										 const std::string& Tex,
										 const D3DXVECTOR3& Position,
										 const std::string shader,
										 BoundingBox aBoundingBox)
:SceneObject(mesh, Tex, Position, shader, aBoundingBox)
{
	m_Anim = anim ; 
}

void SceneObjectAnimated::InitObjectAnimation()
{
	InitObject();
	ResourceManager::GetInstance()->Load<Anim>(m_Anim);
}
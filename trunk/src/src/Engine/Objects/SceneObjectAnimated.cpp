#include "SceneObjectAnimated.h"
#include "Resources/ResourceManager.h"
#include "Resources/Anim.h"

SceneObjectAnimated::SceneObjectAnimated()
:SceneObject()
{
}

SceneObjectAnimated::SceneObjectAnimated(const std::string& mesh, const std::string& anim, const std::string& Tex, const D3DXVECTOR3& Position,  BoundingBox* aBoundingBox, const std::string shader)
:SceneObject(mesh, Tex, Position, aBoundingBox, shader)
{
	m_Anim = anim ; 
}

void SceneObjectAnimated::InitObjectAnimation()
{
	InitObject();
	ResourceManager::GetInstance()->Load<Anim>(m_Anim);
}
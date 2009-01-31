#include "SceneObjectAnimated.h"


SceneObjectAnimated::SceneObjectAnimated()
:SceneObject()
{
}

SceneObjectAnimated::SceneObjectAnimated(const std::string& mesh, const std::string& anim, const std::string& Tex, const D3DXVECTOR3& Position/*, const PhysicalObject PhysicObj*/, const std::string shader)
:SceneObject(mesh, Tex, Position, /*PhysicObj,*/shader)
{
}
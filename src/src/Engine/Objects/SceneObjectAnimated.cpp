#include "SceneObjectAnimated.h"


SceneObjectAnimated::SceneObjectAnimated()
{
}

SceneObjectAnimated::SceneObjectAnimated(const std::string& mesh, const std::string& anim, const std::string& Tex, const D3DXVECTOR3& Position, const std::string shader)
:SceneObject(mesh, Tex, Position, shader)
{
}
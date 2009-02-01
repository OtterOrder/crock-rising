#ifndef		_SceneObjectAnimated_H
#define		_SceneObjectAnimated_H

//******************************************************************

#include	"SceneObject.h"

//******************************************************************

class SceneObjectAnimated : public SceneObject
{
public:
	SceneObjectAnimated();

	SceneObjectAnimated(const std::string& mesh, const std::string& anim, const std::string& Tex, const D3DXVECTOR3& Position,  BoundingBox* aBoundingBox = NULL,const std::string shader = "default_skinning.fx");

private:
	unsigned int m_uCurrentAnim;

protected:
};

//******************************************************************
#endif		// _SceneObjectAnimated_H

#ifndef		_SceneObjectAnimated_H
#define		_SceneObjectAnimated_H

//******************************************************************

#include	"SceneObject.h"

class Anim ; 

//******************************************************************

class SceneObjectAnimated : public SceneObject
{
public:
	SceneObjectAnimated();

	SceneObjectAnimated(const std::string& mesh,
						const std::string& anim,
						const std::string& Tex,
						const D3DXVECTOR3& Position,
						const std::string shader = "default_skinning.fx",
						BoundingBox aBoundingBox = BoundingBox());

	void InitObjectAnimation();
private:
	unsigned int m_uCurrentAnim;
	std::string			m_Anim;

protected:
};

//******************************************************************
#endif		// _SceneObjectAnimated_H

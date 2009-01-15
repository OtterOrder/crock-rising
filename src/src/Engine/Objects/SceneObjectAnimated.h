#ifndef		_SceneObjectAnimated_H
#define		_SceneObjectAnimated_H

//******************************************************************

#include	"SceneObject.h"

//******************************************************************

class SceneObjectAnimated : public SceneObject
{
public:
	SceneObjectAnimated();

private:
	unsigned int m_uCurrentAnim;

protected:
};

//******************************************************************
#endif		// _SceneObjectAnimated_H

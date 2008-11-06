#ifndef		_Camera_H
#define		_Camera_H

#include	"Object.h"

class Camera : public Object
{
public:
	Camera(void);
	Camera( Vector3f pos, Vector3f up, Vector3f target );

private:
	Vector3f m_Up;
	Vector3f m_Target;
};

#endif		// _Camera_H

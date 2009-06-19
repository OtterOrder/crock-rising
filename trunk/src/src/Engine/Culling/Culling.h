#pragma once

#include "Objects/SceneObject.h"
#include "Objects/Camera.h"

#include <list>
using namespace std;

class Culling
{
public:
	Culling() {};
	virtual ~Culling() {};

	virtual void Process (list<SceneObject*> _SceneObjectList, Camera* _Camera, list<SceneObject*>& _SortedList) =0;
};

/*******************************************************************************************************/
struct Plane 
{
	Vector3f m_position;
	Vector3f m_normal;

	void SetPositionAndNormal (Vector3f _position, Vector3f _normal)	{ m_position = _position; m_normal = _normal; };
};

class Culling3D : public Culling
{
	// ToDo : - accèder au points de la géomètrie englobante
	//        - savoir si le frustrum est englobé dans la géomètrie

public:
	Culling3D () {};

	virtual void Process (list<SceneObject*> _SceneObjectList, Camera* _Camera, list<SceneObject*>& _SortedList);

private:
	Plane m_fustrumShape [6];

	void ComputeCameraFustrumShape (Camera* _Camera);
	bool PointInsideFrustrum (Vector3f _Point);
	bool SphereInsideFrustrum (Vector3f _Center, float _Radius);
};

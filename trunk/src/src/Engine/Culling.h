#pragma once

#include "Physics/BoundingBox.h"
#include "Objects/Camera.h"

#include <list>
using namespace std;

class Culling
{
public:
	Culling() {};
	virtual ~Culling() {};

	virtual void Process (list<SceneObjectPhysics*> _SceneObjectList, Camera* _Camera, list<SceneObjectPhysics*>& _SortedList) =0;
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
	// ToDo : - acc�der au points de la g�om�trie englobante
	//        - savoir si le frustrum est englob� dans la g�om�trie

public:
	Culling3D () {};

	virtual void Process (list<SceneObjectPhysics*> _SceneObjectList, Camera* _Camera, list<SceneObjectPhysics*>& _SortedList);

private:
	Plane m_fustrumShape [6];

	void ComputeCameraFustrumShape (Camera* _Camera);
	BOOL PointInsideFrustrum (Vector3f _Point);
};

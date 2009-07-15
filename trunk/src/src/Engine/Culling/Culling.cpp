#include "Culling.h"
#include "Physics/Physicalizer.h"

void Culling3D::Process (list<SceneObject*> _SceneObjectList, Camera* _Camera, list<SceneObject*>& _SortedList)
{
	ComputeCameraFustrumShape(_Camera);

	std::list<SceneObject*>::iterator scnObjPhysics = _SceneObjectList.begin();
	while (scnObjPhysics != _SceneObjectList.end())
	{
		ShapeType type = (*(*scnObjPhysics)->getPhysicBodyList().begin())->shapeType;
		NxActor* pActor = (*scnObjPhysics)->getActor();

		if ( ActorInsideFrustrum(pActor) )
		{
			_SortedList.push_back(*scnObjPhysics);
		}

		scnObjPhysics ++;
	}
}

void Culling3D::ComputeCameraFustrumShape (Camera* _Camera)
{
	if (!_Camera)
		return;

	#define ANG2RAD 3.14159265358979323846/180.0
	Vector3f nc,fc,X,Y,Z,p;
	float nearD, farD, nw,nh;

	nearD = _Camera->GetZNear();
	farD  = _Camera->GetZFar();

	p = _Camera->GetPosition();

	Z = p - _Camera->GetTarget();
	D3DXVec3Normalize(&Z, &Z);

	D3DXVec3Cross(&X, &_Camera->GetUp(), &Z);			//X = Up * Z
	D3DXVec3Normalize(&X, &X);

	D3DXVec3Cross(&Y, &Z, &X);							//Y = Z * X

	nc = p - (Z*nearD);
	fc = p - (Z*farD);

	m_fustrumShape[0].SetPositionAndNormal(nc, -Z);					//pl[NEARP].setNormalAndPoint(-Z,nc);
	m_fustrumShape[1].SetPositionAndNormal(fc, Z);					//pl[FARP].setNormalAndPoint(Z,fc);

	// compute width and height of the near and far plane sections
	float tang = (float)tan(ANG2RAD * _Camera->GetFOV() * 0.5) ;
	nh = nearD * tang;
	nw = nh * _Camera->GetRatio(); 

	Vector3f aux,normal;

	aux = (nc + Y*nh) - p;
	D3DXVec3Normalize(&aux, &aux);
	D3DXVec3Cross(&normal, &aux, &X);					//normal = aux * X

	m_fustrumShape[2].SetPositionAndNormal(nc+(Y*nh), normal);		//pl[TOP].setNormalAndPoint(normal,nc+Y*nh);

	aux = (nc - Y*nh) - p;
	D3DXVec3Normalize(&aux, &aux);
	D3DXVec3Cross(&normal, &X, &aux);					//normal = X * aux;
	m_fustrumShape[3].SetPositionAndNormal(nc-(Y*nh), normal);		//pl[BOTTOM].setNormalAndPoint(normal,nc-Y*nh);

	aux = (nc - X*nw) - p;
	D3DXVec3Normalize(&aux, &aux);
	D3DXVec3Cross(&normal, &aux, &Y);					//normal = aux * Y;
	m_fustrumShape[4].SetPositionAndNormal(nc-(X*nw), normal);		//pl[LEFT].setNormalAndPoint(normal,nc-X*nw);

	aux = (nc + X*nw) - p;
	D3DXVec3Normalize(&aux, &aux);
	D3DXVec3Cross(&normal, &Y, &aux);					//normal = Y * aux;
	m_fustrumShape[5].SetPositionAndNormal(nc+(X*nw), normal);		//pl[RIGHT].setNormalAndPoint(normal,nc+X*nw);
}

bool Culling3D::ActorInsideFrustrum (NxActor* _pActor)
{
	NxShape*const* shapes = _pActor->getShapes();
	u32 nbShapes = _pActor->getNbShapes();

	NxShapeType shapeType;

	for (u32 shape = 0; shape < nbShapes; shape++)
	{
		shapeType = shapes[shape]->getType();

		if (shapeType == NX_SHAPE_SPHERE)
		{
			NxSphereShape* sphereShape = shapes[shape]->isSphere();
			Vector3f center = NxVec3ToVector3f(sphereShape->getGlobalPosition());

			if (SphereInsideFrustrum(center, sphereShape->getRadius()))
				return true;
		}
		else
		if (shapeType == NX_SHAPE_BOX)
		{
			NxBoxShape* boxShape = shapes[shape]->isBox();

			if (BoxInsideFrsutrum(boxShape->getGlobalPosition(), boxShape->getGlobalOrientation(), boxShape->getDimensions()))
				return true;
		}
		else
		if (shapeType == NX_SHAPE_CAPSULE)
		{
			NxCapsuleShape* capsuleShape = shapes[shape]->isCapsule();

			if (CapsuleInsideFrustrum(capsuleShape->getGlobalPosition(), capsuleShape->getGlobalOrientation(), capsuleShape->getHeight(), capsuleShape->getRadius()))
				return true;
		}
	}

	return false;
}

bool Culling3D::PointInsideFrustrum (Vector3f _Point)
{
	for (u8 frustrumPlane = 0; frustrumPlane < 6; frustrumPlane ++ )
	{
		float NDotV = D3DXVec3Dot(&(m_fustrumShape[frustrumPlane].m_normal), &(_Point - m_fustrumShape[frustrumPlane].m_position));
		if (NDotV < 0.f)
			return false;
	}

	return true;
}

bool Culling3D::SphereInsideFrustrum (Vector3f _Center, float _Radius)
{
	for (u8 frustrumPlane = 0; frustrumPlane < 6; frustrumPlane ++ )
	{
		float NDotV = D3DXVec3Dot(&(m_fustrumShape[frustrumPlane].m_normal), &(_Center - m_fustrumShape[frustrumPlane].m_position));
		if (NDotV < -_Radius)
			return false;
	}

	return true;
}

bool Culling3D::BoxInsideFrsutrum (NxVec3 _Center, NxMat33 _Orientation, NxVec3 _Dimensions)
{
	NxVec3 boxPoints [8];

	_Dimensions /= 2.f;

	boxPoints[0] = _Center + NxVec3(-_Dimensions.x, -_Dimensions.y, -_Dimensions.x);
	boxPoints[1] = _Center + NxVec3( _Dimensions.x, -_Dimensions.y, -_Dimensions.x);
	boxPoints[2] = _Center + NxVec3(-_Dimensions.x,  _Dimensions.y, -_Dimensions.x);
	boxPoints[3] = _Center + NxVec3( _Dimensions.x,  _Dimensions.y, -_Dimensions.x);
	boxPoints[4] = _Center + NxVec3(-_Dimensions.x, -_Dimensions.y,  _Dimensions.x);
	boxPoints[5] = _Center + NxVec3( _Dimensions.x, -_Dimensions.y,  _Dimensions.x);
	boxPoints[6] = _Center + NxVec3(-_Dimensions.x,  _Dimensions.y,  _Dimensions.x);
	boxPoints[7] = _Center + NxVec3( _Dimensions.x,  _Dimensions.y,  _Dimensions.x);

	for (u32 i=0; i<8; i++)
	{
		_Orientation.multiply(boxPoints[i], boxPoints[i]);
		if (PointInsideFrustrum( NxVec3ToVector3f(boxPoints[i]) ))
			return true;
	}

	return false;
}

bool Culling3D::CapsuleInsideFrustrum (NxVec3 _Center, NxMat33 _Orientation, float _Height, float _Radius)
{
	NxVec3 dimensions (_Radius, _Height, _Radius);

	return BoxInsideFrsutrum(_Center, _Orientation, dimensions);
}
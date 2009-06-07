#include "Culling.h"

void Culling3D::Process (list<SceneObjectPhysics*> _SceneObjectList, Camera* _Camera, list<SceneObjectPhysics*>& _SortedList)
{
	ComputeCameraFustrumShape(_Camera);

	std::list<SceneObjectPhysics*>::iterator scnObjPhysics = _SceneObjectList.begin();
	while (scnObjPhysics != _SceneObjectList.end())
	{
		// parcours des points de la g�ometrie englobante
		//for (u32 i=0; i<nbPoints; i++)
		{
			//if ( PointInsideFrustrum(currentPoint))
			{
				_SortedList.push_back(*scnObjPhysics);
				continue;
			}
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

BOOL Culling3D::PointInsideFrustrum (Vector3f _Point)
{
	for (u8 frustrumPlane = 0; frustrumPlane < 6; frustrumPlane ++ )
	{
		if (D3DXVec3Dot(&(m_fustrumShape[frustrumPlane].m_normal), &(_Point - m_fustrumShape[frustrumPlane].m_position)) < 0.f)
			return FALSE;
	}

	return TRUE;
}
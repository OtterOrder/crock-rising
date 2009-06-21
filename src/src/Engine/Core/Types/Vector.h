#ifndef		_Vector_H
#define		_Vector_H

#include	"Core/Types/Types.h"
#include	<d3dx9math.h>

#include	"NxPhysics.h"

//******************************************************************

#define		VEC_PRECISION		0.00001f		// Précision pour les vecteurs

//******************************************************************
// Vecteur 2f

typedef D3DXVECTOR2 Vector2f, Point2f;

bool VEC_AreAlmostEqual( const Vector2f &v1, const Vector2f &v2 );


//******************************************************************
// Vecteur 3f

typedef D3DXVECTOR3 Vector3f, Point3f;

bool VEC_AreAlmostEqual( const Vector3f &v1, const Vector3f &v2 );


//******************************************************************
// Vecteur 4f

typedef D3DXVECTOR4 Vector4f, Point4f;

bool VEC_AreAlmostEqual( const Vector4f &v1, const Vector4f &v2 );


//******************************************************************
// Vecteur 2i (utile en 2D)

struct Vector2i
{
	s32 x, y;

	Vector2i() {}
	Vector2i( s32 _x, s32 _y );
	Vector2i( const Vector2i &_v );
	Vector2i( const Vector2f &_v );

	operator Vector2f();

};


//******************************************************************
// Vector3f to NxVec3

Vector3f NxVec3ToVecotr3f (const NxVec3& _v);

#endif		// _Vector_H

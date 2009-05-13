#ifndef		_Vector_H
#define		_Vector_H

#include	"Core/Types/Types.h"
#include	<d3dx9math.h>

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

struct Coord
{
	s32 x;
	s32 y;
	Coord (s32 _x =0, s32 _y =0) { x = _x; y = _y; };
};

#endif		// _Vector_H

#include "Vector.h"

//******************************************************************

/***********************************************************
* V�rifie si les vecteurs (2f) sont presque �gaux.
* @param[in]	v1	: vecteur 1
* @param[in]	v2	: vecteur 2
* @return	true si les vecteurs sont presque �gaux, false
*			sinon
**********************************************************/
bool VEC_AreAlmostEqual( const Vector2f &v1, const Vector2f &v2 )
{
	return ( fabs( v1.x - v2.x ) < VEC_PRECISION )
		&& ( fabs( v1.y - v2.y ) < VEC_PRECISION );
}

/***********************************************************
* V�rifie si les vecteurs (3f) sont presque �gaux.
* @param[in]	v1	: vecteur 1
* @param[in]	v2	: vecteur 2
* @return	true si les vecteurs sont presque �gaux, false
*			sinon
**********************************************************/
bool VEC_AreAlmostEqual( const Vector3f &v1, const Vector3f &v2 )
{
	return ( fabs( v1.x - v2.x ) < VEC_PRECISION )
		&& ( fabs( v1.y - v2.y ) < VEC_PRECISION )
		&& ( fabs( v1.z - v2.z ) < VEC_PRECISION );
}

/***********************************************************
* V�rifie si les vecteurs (4f) sont presque �gaux.
* @param[in]	v1	: vecteur 1
* @param[in]	v2	: vecteur 2
* @return	true si les vecteurs sont presque �gaux, false
*			sinon
**********************************************************/
bool VEC_AreAlmostEqual( const Vector4f &v1, const Vector4f &v2 )
{
	return ( fabs( v1.x - v2.x ) < VEC_PRECISION )
		&& ( fabs( v1.y - v2.y ) < VEC_PRECISION )
		&& ( fabs( v1.z - v2.z ) < VEC_PRECISION )
		&& ( fabs( v1.w - v2.w ) < VEC_PRECISION );
}

//**********************************************************
// Constructeur de Vector2i avec param�tres.
// @param[in]	_x	: coordonn�e x
// @param[in]	_y	: coordonn�e y
//**********************************************************
Vector2i::Vector2i( s32 _x, s32 _y )
{
	x = _x;
	y = _y;
}

//**********************************************************
// Constructeur de Vector2i par copie.
// @param[in]	_v	: vecteur
//**********************************************************
Vector2i::Vector2i( const Vector2i &_v )
{
	x = _v.x;
	y = _v.y;
}

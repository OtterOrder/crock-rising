#include "Vector.h"

//******************************************************************

/***********************************************************
* Vérifie si les vecteurs (2f) sont presque égaux.
* @param[in]	v1	: vecteur 1
* @param[in]	v2	: vecteur 2
* @return	true si les vecteurs sont presque égaux, false
*			sinon
**********************************************************/
bool VEC_AreAlmostEqual( const Vector2f &v1, const Vector2f &v2 )
{
	return ( fabs( v1.x - v2.x ) < VEC_PRECISION )
		&& ( fabs( v1.y - v2.y ) < VEC_PRECISION );
}

/***********************************************************
* Vérifie si les vecteurs (3f) sont presque égaux.
* @param[in]	v1	: vecteur 1
* @param[in]	v2	: vecteur 2
* @return	true si les vecteurs sont presque égaux, false
*			sinon
**********************************************************/
bool VEC_AreAlmostEqual( const Vector3f &v1, const Vector3f &v2 )
{
	return ( fabs( v1.x - v2.x ) < VEC_PRECISION )
		&& ( fabs( v1.y - v2.y ) < VEC_PRECISION )
		&& ( fabs( v1.z - v2.z ) < VEC_PRECISION );
}

/***********************************************************
* Vérifie si les vecteurs (4f) sont presque égaux.
* @param[in]	v1	: vecteur 1
* @param[in]	v2	: vecteur 2
* @return	true si les vecteurs sont presque égaux, false
*			sinon
**********************************************************/
bool VEC_AreAlmostEqual( const Vector4f &v1, const Vector4f &v2 )
{
	return ( fabs( v1.x - v2.x ) < VEC_PRECISION )
		&& ( fabs( v1.y - v2.y ) < VEC_PRECISION )
		&& ( fabs( v1.z - v2.z ) < VEC_PRECISION )
		&& ( fabs( v1.w - v2.w ) < VEC_PRECISION );
}

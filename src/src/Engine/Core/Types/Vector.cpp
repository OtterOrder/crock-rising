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

//**********************************************************
// Constructeur de Vector2i avec paramètres.
// @param[in]	_x	: Coordonnée x
// @param[in]	_y	: Coordonnée y
//**********************************************************
Vector2i::Vector2i( s32 _x, s32 _y )
{
	x = _x;
	y = _y;
}

//**********************************************************
// Constructeur de Vector2i par copie.
// @param[in]	_v	: Vecteur
//**********************************************************
Vector2i::Vector2i( const Vector2i &_v )
{
	x = _v.x;
	y = _v.y;
}

//**********************************************************
// Constructeur de Vector2i par copie d'un Vector2f.
// @param[in]	_v	: Vecteur
//**********************************************************
Vector2i::Vector2i( const Vector2f &_v )
{
	x = (s32)_v.x;
	y = (s32)_v.y;
}

//**********************************************************
// Cast en Vector2f. Applique un cast standard int -> float.
// @return	Le vecteur sous forme de Vector2f
//**********************************************************
Vector2i::operator Vector2f()
{
	Vector2f v;
	v.x = (float)x;
	v.y = (float)y;
	return v;
}


//**********************************************************
// Cast un NxVect3 en Vector3f.
// @return	Le vecteur sous forme de Vector3f
//**********************************************************
Vector3f NxVec3ToVector3f (const NxVec3& _v)
{
	return Vector3f(_v.x, _v.y, _v.z);
}
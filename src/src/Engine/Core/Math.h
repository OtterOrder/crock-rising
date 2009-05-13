#ifndef		_CROCKENGINE_MATH_H_
#define		_CROCKENGINE_MATH_H_

//******************************************************************

#include	<math.h>
#include	<d3dx9math.h>

#include	"Types/Types.h"
#include	"Types/Crc32.h"
#include	"Types/Vector.h"
#include	"Types/Color.h"

//******************************************************************

// Ne pas mettre n'importe quoi "pour tester", car ce fichier
// est amené à être inclus dans plein d'autres ; toute modification
// devrait pourrir le temps de compilation, donc ça a intéret de
// valoire le coup..

// Merci de respecter la même syntaxe pour les macros et
// fonctions de math, à savoir : MATH_MyFunction().

template< typename T >
inline const T& MATH_Clamp( const T& x, const T& min, const T& max )
{
	return ( x < min ) ? min : ( x > max ) ? max : x;
}

//******************************************************************

// Manipulation de flags facile
// @param field	: champs de flag sur lequel on travaille
// @param flag	: flag à activer, désactiver, tester, etc.

// Note: pour faire des flags simplement, il suffit de
// définir des constantes qui sont des puissances de 2 :
// MY_FLAG_A = 1<<0 (->001)
// MY_FLAG_B = 1<<1 (->010)
// MY_FLAG_C = 1<<2 (->100)
// etc.

#define FLAG_Set( field, flag )		((field)|=(flag))
#define FLAG_Unset( field, flag )	((field)^=(flag))
#define FLAG_IsSet( field, flag )	(((field)&(flag))==(flag))

//******************************************************************
#endif		//_CROCKENGINE_MATH_H_

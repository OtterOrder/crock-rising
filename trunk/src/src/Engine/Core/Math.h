#ifndef		_Math_H_CrockEngine
#define		_Math_H_CrockEngine

//******************************************************************

#include	<math.h>
//#include	<D3DX9Math.h> // utile ?

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

#define	MATH_Clamp( x, min, max )	(((x)<(min))?(min):(((x)>(max))?(max):(x)))

//******************************************************************
#endif		//_Math_H_CrockEngine

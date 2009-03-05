#ifndef		_Mask_H
#define		_Mask_H

//******************************************************************

#include	"Object2D.h"

//******************************************************************

class Mask : public Object2D
{
public:

	//-- M�thodes publiques
	
	Mask( float sizeX, float sizeY, const Color4f &color );
	Mask( const Vector2f &size, const Color4f &color );
	virtual ~Mask();

	virtual void Draw() const;		// Affiche l'objet

protected:

	//TODO

};

//******************************************************************
#endif		//_Mask_H
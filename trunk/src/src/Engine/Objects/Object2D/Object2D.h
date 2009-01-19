#ifndef		_Object2D_H
#define		_Object2D_H

//******************************************************************

#include	"Core/Types/Crc32.h"
#include	"Core/Types/Vector.h"

//******************************************************************

class Object2D
{
public:

	Object2D				( void );
	virtual ~Object2D		( void );

	virtual void	Draw	( void ) const =0;				// Affiche l'objet

	void		SetPosition	( float posX, float posY );		// Change la position
	void		SetPosition	( const Point2f &position );	// Change la position
	Point2f		GetPosition	( void ) const;					// Donne la position

	void		SetDepth	( float depth );				// Change la profondeur (z)
	float		GetDepth	( void );						// Donne la profondeur (z)

	void		Set3DPosition	( float posX, float posY, float posZ );// Change la position 3D
	void		Set3DPosition	( const Point3f &position );// Change la position 3D
	Point3f		Get3DPosition	( void ) const;				// Donne la position 3D


protected:

	Point3f		m_Position;			// Position

};

//******************************************************************
#endif		//_Object2D_H

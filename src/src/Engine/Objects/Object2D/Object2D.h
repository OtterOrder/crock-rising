#ifndef		_Object2D_H
#define		_Object2D_H

//******************************************************************

#include	<list>

#include	"Core/Types/Crc32.h"
#include	"Core/Types/Vector.h"

//******************************************************************

class Object2D
{
public:

	// Liste de références sur les objets 2D
	static std::list< Object2D* > RefList;
	
	// =========================================================
	// Méthodes publiques
	
	Object2D();
	virtual ~Object2D();

	virtual void Draw() const =0;								// Affiche l'objet
	virtual void Update();										// Update

	void SetPosition( float posX, float posY );					// Change la position
	void SetPosition( const Point2f &position );				// Change la position
	Point2f GetPosition() const;								// Donne la position

	void SetDepth( float depth );								// Change la profondeur (z)
	float GetDepth();											// Donne la profondeur (z)

	void Set3DPosition( float posX, float posY, float posZ );	// Change la position 3D
	void Set3DPosition( const Point3f &position );				// Change la position 3D
	Point3f Get3DPosition() const;								// Donne la position 3D


protected:

	// =========================================================
	// Données protégées
	
	Point3f		m_Position;			// Position

private:

	// =========================================================
	// Méthodes privées
	
	void CommonInit();	// Initialisation commune aux constructeurs

};

//******************************************************************
#endif		//_Object2D_H

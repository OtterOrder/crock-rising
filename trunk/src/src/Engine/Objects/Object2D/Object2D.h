#ifndef		_Object2D_H
#define		_Object2D_H

//******************************************************************

#include	<list>

#include	"Core/Math.h"

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
	virtual void Update();										// Update, à surcharger

	void SetPosition( float posX, float posY );					// Change la position
	void SetPosition( const Point2f &position );				// Change la position
	Point2f GetPosition() const;								// Donne la position

	//void Set3DPosition( float posX, float posY, float posZ );	// Change la position 3D
	//void Set3DPosition( const Point3f &position );				// Change la position 3D
	//Point3f Get3DPosition() const;								// Donne la position 3D

	void SetVisibility( bool isVisible );						// Rend visible/invisible l'objet
	bool IsVisible() const;										// Vérifie si l'objet est visible

	// Méthodes spécifiques pour DirectX, appelées lors
	// de la création/destruction/perte du device Dx.
	virtual void InitDxData(){}									// Initialise les données Dx
	virtual void ClearDxData(){}								// Libère les données Dx


protected:

	// =========================================================
	// Données protégées
	
	Point3f		m_Position;			// Position
	bool		m_IsVisible;		// Si l'objet est visible

private:

	// =========================================================
	// Méthodes privées
	
	void CommonInit();	// Initialisation commune aux constructeurs

};

//******************************************************************
#endif		//_Object2D_H

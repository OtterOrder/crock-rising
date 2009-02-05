#ifndef		_Object2D_H
#define		_Object2D_H

//******************************************************************

#include	<list>

#include	"Core/Math.h"

//******************************************************************

class Object2D
{
public:

	// Liste de r�f�rences sur les objets 2D
	static std::list< Object2D* > RefList;
	
	// =========================================================
	// M�thodes publiques
	
	Object2D();
	virtual ~Object2D();

	virtual void Draw() const =0;								// Affiche l'objet
	virtual void Update();										// Update, � surcharger

	void SetPosition( float posX, float posY );					// Change la position
	void SetPosition( const Point2f &position );				// Change la position
	Point2f GetPosition() const;								// Donne la position

	//void Set3DPosition( float posX, float posY, float posZ );	// Change la position 3D
	//void Set3DPosition( const Point3f &position );				// Change la position 3D
	//Point3f Get3DPosition() const;								// Donne la position 3D

	void SetVisibility( bool isVisible );						// Rend visible/invisible l'objet
	bool IsVisible() const;										// V�rifie si l'objet est visible

	// M�thodes sp�cifiques pour DirectX, appel�es lors
	// de la cr�ation/destruction/perte du device Dx.
	virtual void InitDxData(){}									// Initialise les donn�es Dx
	virtual void ClearDxData(){}								// Lib�re les donn�es Dx


protected:

	// =========================================================
	// Donn�es prot�g�es
	
	Point3f		m_Position;			// Position
	bool		m_IsVisible;		// Si l'objet est visible

private:

	// =========================================================
	// M�thodes priv�es
	
	void CommonInit();	// Initialisation commune aux constructeurs

};

//******************************************************************
#endif		//_Object2D_H

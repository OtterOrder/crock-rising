#ifndef		_OBJECT2D_H_
#define		_OBJECT2D_H_

//******************************************************************

#include	<list>
#include	"Core/Math.h"

//******************************************************************

// Flags des objets 2D
//#define		O2D_RELATIVE_POS			(1<<0)		// Position relative à la fenêtre
//#define		O2D_RELATIVE_SIZE			(1<<1)		// Taille relative aux dimensions de la fenêtre
#define		O2D_BACKGROUND				(1<<2)		// L'objet est un background

//******************************************************************

class Object2D
{
public:

	// Liste de références sur les objets 2D
	static std::list< Object2D* > RefList;
	
	//-- Méthodes publiques
	
	Object2D();
	virtual ~Object2D();

	// Update. A surcharger
	virtual void Update() {}

	// Position
	void SetPosition( s32 posX, s32 posY );
	void SetPosition( const Vector2i &position );
	void SetPosition( const Vector2f &position );
	Vector2i GetPosition() const;

	//void SetPositionToAbsolute();
	//void SetPositionToRelative();
	
	// Echelle
	void SetScale( float scaleX, float scaleY );
	void SetScale( const Vector2f &scale );
	Vector2f GetScale() const;

	// Rotation
	void SetRotation( float angle );
	float GetRotation() const;

	// Update toute la 2D
	static void FullUpdate();

protected:

	Vector3f		m_Position;			// Position (= translation)
	Vector3f		m_Scale;			// Echelle
	Vector3f		m_Rotation;			// Rotation
	u32				m_Flags;			// Flags
	bool			m_Dirty;			// Indiquateur crade

	//-- Méthodes protégées

	//**********************************************************
	// Active le "dirty" : indique à l'objet qu'il doit mettre
	// à jour des trucs. Cette méthode est tellement crade qu'elle
	// ne suit même pas la nomenclature standard..
	//**********************************************************
	inline void activate_dirty() { m_Dirty = true; }

	virtual void dirty_Refresh() {}					// Met à jour tout le bazar. A surcharger
	void WorldMatrix( D3DMATRIX *pMatrix ) const;	// Calcule la matrice de l'objet dans le repère world

	// Gestion des flags
	void SetFlag( u32 flag );
	void UnsetFlag( u32 flag );
	bool IsFlagSet( u32 flag ) const;

};

//******************************************************************
#endif		//_OBJECT2D_H_

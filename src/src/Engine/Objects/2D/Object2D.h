#ifndef		_OBJECT2D_H_
#define		_OBJECT2D_H_

//******************************************************************

#include	<list>
#include	"Core/Math.h"

//******************************************************************

// Flags pour les objets 2D
//#define		O2D_WINDOW_SIZE				(1<<0)		// L'objet a la taille de la fen�tre
//#define		O2D_ALWAYS_ON_CENTER		(1<<1)		// Point chaud toujours au centre

//******************************************************************

class Object2D
{
public:

	// Liste de r�f�rences sur les objets 2D
	static std::list< Object2D* > RefList;
	
	//-- M�thodes publiques
	
	Object2D();
	virtual ~Object2D();

	// Update. A surcharger
	virtual void Update() {}

	// Position
	void SetPosition( s32 posX, s32 posY );
	void SetPosition( const Vector2i &position );
	void SetPosition( const Vector2f &position );
	Vector2i GetPosition() const;
	
	// Echelle
	void SetScale( float scaleX, float scaleY );
	void SetScale( const Vector2f &scale );
	Vector2f GetScale() const;

	// Rotation
	void SetRotation( float angle );
	float GetRotation() const;

	// Gestion des flags
	void SetFlag( u32 flag );
	void UnsetFlag( u32 flag );
	bool IsFlagSet( u32 flag ) const;

	// Update toute la 2D
	static void FullUpdate();

protected:

	Vector3f		m_Position;			// Position (= translation)
	Vector3f		m_Scale;			// Echelle
	Vector3f		m_Rotation;			// Rotation
	u32				m_Flags;			// Flags
	bool			m_Dirty;			// Indiquateur crade

	//-- M�thodes prot�g�es

	//**********************************************************
	// Active le "dirty" : indique � l'objet qu'il doit mettre
	// � jour des trucs. Cette m�thode est tellement crade qu'elle
	// ne suit m�me pas la nomenclature standard..
	//**********************************************************
	inline void activate_dirty() { m_Dirty = true; }

	virtual void dirty_Refresh() {}					// Met � jour tout le bazar. A surcharger
	void WorldMatrix( D3DMATRIX *pMatrix ) const;	// Calcule la matrice de l'objet dans le rep�re world

};

//******************************************************************
#endif		//_OBJECT2D_H_

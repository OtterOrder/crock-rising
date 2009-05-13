#ifndef		_Background_H_
#define		_Background_H_

//******************************************************************

#include	"Sprite.h"

//******************************************************************

class Background : public Sprite
{
public:

	//-- Méthodes publiques
	
	Background( const char *path );
	virtual ~Background();

	//-- Méthodes spécifiques pour DirectX
	
	virtual void InitDxData();		// Initialise les données Dx

protected:

	//TODO

};

//******************************************************************
#endif		//_Background_H_

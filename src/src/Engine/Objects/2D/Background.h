#ifndef		_Background_H_
#define		_Background_H_

//******************************************************************

#include	"Sprite.h"

//******************************************************************

class Background : public Sprite
{
public:

	//-- M�thodes publiques
	
	Background( const char *path );
	virtual ~Background();

	//-- M�thodes sp�cifiques pour DirectX
	
	virtual void InitDxData();		// Initialise les donn�es Dx

protected:

	//TODO

};

//******************************************************************
#endif		//_Background_H_

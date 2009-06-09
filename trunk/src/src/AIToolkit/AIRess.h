#ifndef _AIRess_H_
#define _AIRess_H_

#include	<Core/Types/Vector.h>

class AIRess
{
public:
	// Variable
	enum statesGZip {	IDLE, MOVE, ATTACK, EVADE	};

public:
	// Function
	static int calculDistance( Vector3f pos1, Vector3f pos2 );
};



#endif
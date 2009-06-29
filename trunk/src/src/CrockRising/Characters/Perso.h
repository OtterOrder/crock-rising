#ifndef		_Perso_H_
#define		_Perso_H_

#include		<vector>
#include		<Objects/SceneObjectAnimated.h>
#include		<Core/Types/Vector.h>
#include		<Physics/Physicalizer.h>

enum PersoState 
{
	STATIC,	
	WALK, 
	RUN,
	ATTACK
};

enum SolidType
{
	WEAPON,
	NOT_WEAPON
};

class Perso
{
public:	
	Perso(){ m_iLife = 100; }
	~Perso(){}

	virtual void		    changeState( PersoState newState ){}
	virtual void			Init(){}
	
	SceneObjectAnimated* 	getSceneObjectAnimated () const { return m_pAnimated; }
	int						&Life(){	return m_iLife;	}
	
protected:
	SceneObjectAnimated*	m_pAnimated;
	int				    	m_iLife;
	PersoState				m_currentState;
};


class ContactReportCR : public ContactReport
{
public:
	virtual void onContactNotify(NxContactPair& pair, NxU32 events);

};

#endif

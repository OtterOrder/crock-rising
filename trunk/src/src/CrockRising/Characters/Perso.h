#ifndef		_Perso_H_
#define		_Perso_H_

#include		<vector>
#include		<Objects/SceneObjectAnimated.h>
#include		<Core/Types/Vector.h>
#include		<Physics/Physicalizer.h>
#include		<Resources/Material.h>

enum PersoState 
{
	STATIC,	
	RUN,
	ATTACK,
	DIE,
	HIT
};

enum SolidType
{
	WEAPON,
	NOT_WEAPON
};

class Perso
{
public:	
	Perso(){ }
	~Perso(){}

	virtual void		    changeState( PersoState newState ){}
	virtual void			Init(){}
	
	SceneObjectAnimated* 	getSceneObjectAnimated () const { return m_pAnimated; }
	
	int						&Life() { return m_iLife; }
	void					setLife ( const int decreaseLife ); 
	
protected:
	SceneObjectAnimated*	m_pAnimated;
	int					    m_iLife;
	PersoState				m_currentState;
};


class ContactReportCR : public ContactReport
{
public:
	virtual void onContactNotify(NxContactPair& pair, NxU32 events);

};

#endif

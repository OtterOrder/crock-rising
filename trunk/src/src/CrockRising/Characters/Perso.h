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
	Perso(): m_iLife(50), m_StateFrozen(false){}
	~Perso(){}

	virtual void		    changeState( PersoState newState )=0;
	virtual void			Init()=0;
	virtual void			Hit()=0;
	virtual void			Die()=0;
	
	SceneObjectAnimated* 	getSceneObjectAnimated () const { return m_pAnimated; }
	PersoState				getCurrentState(){ return m_currentState; }
	
	int						&Life() { return m_iLife; }
	void					decLife ( const int decreaseLife );
	void					DestroyPerso();
	void					FreezeState(){ m_StateFrozen = true; }
	void					UnFreezeState(){ m_StateFrozen = false; }
	
protected:
	SceneObjectAnimated*	m_pAnimated;
	int					    m_iLife;
	PersoState				m_currentState;
	bool					m_StateFrozen;
};


class ContactReportCR : public ContactReport
{
public:
	virtual void onContactNotify(NxContactPair& pair, NxU32 events);

};

#endif

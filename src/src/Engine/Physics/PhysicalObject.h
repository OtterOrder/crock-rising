#pragma once

class PhysicalObject
{
	//list<primitive> m_BoundingBoxList;
	bool m_bDebugMode; //Si vrai, les bounding box se dessinent pour debugger.
public:
	PhysicalObject(void);
	~PhysicalObject(void);

	bool IsInCollision(const PhysicalObject &po);
	void EnableDebugMode(bool enable);
};

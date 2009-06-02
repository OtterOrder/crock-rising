#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "Object.h"
#include <list>

class Shader;

#define MAX_LIGHTS 4

class Light : public Object
{
public:
	// Liste de références sur les lumières
	static std::list< Light* > RefList;
	typedef std::list< Light* >::iterator LightIt;

	//===========================================================================//
	// Constructeur									                             //
	//===========================================================================//
	Light();
	virtual ~Light();

	//===========================================================================//
	// Propriétés de la lumière    							                     //
	//===========================================================================//
protected:
	static	D3DXVECTOR4		m_LightsColor[MAX_LIGHTS]; 
	static	D3DXVECTOR4		m_LightsSpecular[MAX_LIGHTS];
	static	D3DXVECTOR3		m_LightsPosition[MAX_LIGHTS];
	static  D3DXVECTOR3		m_LightsDirection[MAX_LIGHTS];
	static	float			m_LightsAngle[MAX_LIGHTS];
	static  float			m_LightsAttenuation[MAX_LIGHTS];
	static  float			m_LightsExponent[MAX_LIGHTS];


	int				m_LightId;

public:
	virtual void SetLightData(Shader * shader);
	void		 SetLightColor(D3DXVECTOR4 Color);
	void		 SetLightSpecular(D3DXVECTOR4 Color);
	void		 SetLightPosition(D3DXVECTOR3 pos);
	virtual void SetLightAttenuation(float att){}
	virtual void SetLightAngle(float angle){}

	D3DXVECTOR3  GetLightPosition() {return m_LightsPosition[m_LightId];}
	D3DXVECTOR3  GetLightDirection() {return m_LightsDirection[m_LightId];}
	float		 GetLightAngle() {return m_LightsAngle[m_LightId];}

protected:
	void CommonInit();

};

class DirectionalLight : public Light
{
public:

	DirectionalLight();
	virtual ~DirectionalLight();
};


class PointLight : public DirectionalLight
{
public:
	void SetLightAttenuation(float att);
	PointLight();
	virtual ~PointLight();
};


class SpotLight : public DirectionalLight
{
public:
	void SetLightAngle(float angle);
	SpotLight();
	virtual ~SpotLight();

};
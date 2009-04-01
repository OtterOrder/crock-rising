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

	// Liste de r�f�rences sur les lumi�res
	static std::list< Light* > RefList;

	static	int				m_LightsType[MAX_LIGHTS];		// Tableaux de donn�es des lumi�res
	static	D3DXVECTOR4		m_LightsColor[MAX_LIGHTS]; 
	static	D3DXVECTOR4		m_LightsSpecular[MAX_LIGHTS];
	static	D3DXVECTOR3		m_LightsPosition[MAX_LIGHTS];

	//===========================================================================//
	// Constructeur									                             //
	//===========================================================================//
	Light();
	virtual ~Light();

	//===========================================================================//
	// Propri�t�s de la lumi�re    							                     //
	//===========================================================================//
	int				m_LightId;
	D3DXVECTOR4		m_LightColor;
	D3DXVECTOR4		m_SpecularColor;

	virtual void SetLightData(Shader * shader);

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

	float m_Attenuation;

	PointLight();
	virtual ~PointLight();
};


class SpotLight : public DirectionalLight
{
public:

	float m_ConeAngle;

	SpotLight();
	virtual ~SpotLight();

};
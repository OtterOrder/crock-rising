//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "Light.h"
#include "Resources/Shader.h"

// Données membres statiques

std::list< Light* > Light::RefList;

D3DXVECTOR4 Light::m_LightsColor[MAX_LIGHTS];
D3DXVECTOR4 Light::m_LightsSpecular[MAX_LIGHTS];
D3DXVECTOR3 Light::m_LightsPosition[MAX_LIGHTS];
D3DXVECTOR3 Light::m_LightsDirection[MAX_LIGHTS];
float		Light::m_LightsAngle[MAX_LIGHTS];
float		Light::m_LightsAttenuation[MAX_LIGHTS];
float		Light::m_LightsExponent[MAX_LIGHTS];


void Light::CommonInit()
{
	m_LightId=(int)RefList.size();    // Id de la lumière
	Light::RefList.push_back( this ); // enregistrement dans la liste
}
//===========================================================================//
// Constructeur Light   						                             //
//===========================================================================//
Light::Light()
{
	CommonInit();
	m_LightsColor[m_LightId]=D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_LightsSpecular[m_LightId]=D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
}

Light::~Light()
{
}

void Light::SetLightData(Shader * shader)
{
	int size=(int)RefList.size();
	shader->GetEffect()->SetValue( "g_NumLights", (void*)&size, sizeof(int));
	shader->GetEffect()->SetValue( "g_LightsColor", m_LightsColor, sizeof( D3DXVECTOR4 ) * MAX_LIGHTS );
	shader->GetEffect()->SetValue( "g_LightsSpecular", m_LightsSpecular, sizeof( D3DXVECTOR4 ) * MAX_LIGHTS );
	shader->GetEffect()->SetValue( "g_LightsPosition", m_LightsPosition, sizeof( D3DXVECTOR3 ) * MAX_LIGHTS );
	shader->GetEffect()->SetValue( "g_LightsDirection", m_LightsDirection, sizeof( D3DXVECTOR3 ) * MAX_LIGHTS );
	shader->GetEffect()->SetValue( "g_LightsAttenuation", m_LightsAttenuation, sizeof( float ) * MAX_LIGHTS );
	shader->GetEffect()->SetValue( "g_LightsAngle", m_LightsAngle, sizeof( float ) * MAX_LIGHTS );
	shader->GetEffect()->SetValue( "g_LightsExponent", m_LightsExponent, sizeof( float ) * MAX_LIGHTS );

}

void Light::SetLightColor(D3DXVECTOR4 Color)
{
	m_LightsColor[m_LightId]=Color;
}

void Light::SetLightSpecular(D3DXVECTOR4 Color)
{
	m_LightsSpecular[m_LightId]=Color;
}

void Light::SetLightPosition(D3DXVECTOR3 pos)
{
	m_LightsPosition[m_LightId]=pos;
}

//===========================================================================//
// Constructeur DirectionalLight				                             //
//===========================================================================//
DirectionalLight::DirectionalLight()
{
	m_LightsPosition[m_LightId]=D3DXVECTOR3(0.f, 300.f, 0.f);
	m_LightsAngle[m_LightId]=0.f;
	m_LightsAttenuation[m_LightId]=0.f;
	m_LightsDirection[m_LightId]=D3DXVECTOR3(0.f, 0.f, 0.f);
	m_LightsExponent[m_LightId]=0.f;
}

DirectionalLight::~DirectionalLight()
{

}

//===========================================================================//
// Constructeur PointLight						                             //
//===========================================================================//
PointLight::PointLight()
{
	m_LightsPosition[m_LightId]=D3DXVECTOR3(0.f, 100.f, 0.f);
	m_LightsAngle[m_LightId]=0.1f;
	m_LightsAttenuation[m_LightId]=0.5f;
	m_LightsDirection[m_LightId]=D3DXVECTOR3(0.f, 1.f, 0.f);
	m_LightsExponent[m_LightId]=2.f;
}

PointLight::~PointLight()
{

}

void PointLight::SetLightAttenuation(float att)
{
	m_LightsAttenuation[m_LightId]=att;

}

//===========================================================================//
// Constructeur PointLight						                             //
//===========================================================================//
SpotLight::SpotLight()
{
	m_LightsPosition[m_LightId]=D3DXVECTOR3(0.f, 100.f, 0.f);
	m_LightsAngle[m_LightId]=0.65f;
	m_LightsAttenuation[m_LightId]=0.5f;
	m_LightsDirection[m_LightId]=D3DXVECTOR3(0.f, 0.f, 0.f);
	m_LightsExponent[m_LightId]=2.f;
}

SpotLight::~SpotLight()
{

}

void SpotLight::SetLightAngle(float angle)
{
	m_LightsAngle[m_LightId]=angle;
}
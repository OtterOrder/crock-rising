//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "Light.h"
#include "Resources/Shader.h"

// Données membres statiques

std::list< Light* > Light::RefList;

int		    Light::m_LightsType[MAX_LIGHTS];
D3DXVECTOR4 Light::m_LightsColor[MAX_LIGHTS];
D3DXVECTOR4 Light::m_LightsSpecular[MAX_LIGHTS];
D3DXVECTOR3 Light::m_LightsPosition[MAX_LIGHTS];


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
	m_LightsColor[m_LightId]=D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.f);
	m_LightsSpecular[m_LightId]=D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);

}

Light::~Light()
{
	int u=0;
}

void Light::SetLightData(Shader * shader)
{
	int size=(int)RefList.size();
	shader->m_pEffect->SetValue( "g_NumLights", (void*)&size, sizeof(int));
	shader->m_pEffect->SetValue( "g_LightsType", m_LightsType, sizeof( D3DXVECTOR3 ) * MAX_LIGHTS );
	shader->m_pEffect->SetValue( "g_LightsColor", m_LightsColor, sizeof( D3DXVECTOR4 ) * MAX_LIGHTS );
	shader->m_pEffect->SetValue( "g_LightsSpecular", m_LightsSpecular, sizeof( D3DXVECTOR4 ) * MAX_LIGHTS );
	shader->m_pEffect->SetValue( "m_LightsPosition", m_LightsPosition, sizeof( D3DXVECTOR3 ) * MAX_LIGHTS );

}

//===========================================================================//
// Constructeur DirectionalLight				                             //
//===========================================================================//
DirectionalLight::DirectionalLight()
{
	m_LightsType[m_LightId]=0;
	m_LightsPosition[m_LightId]=D3DXVECTOR3(0.f, 100.f, 0.f);
}

DirectionalLight::~DirectionalLight()
{

}

//===========================================================================//
// Constructeur PointLight						                             //
//===========================================================================//
PointLight::PointLight()
{
	m_LightsPosition[m_LightId]=D3DXVECTOR3(0.f, 100.f, -100.f);
	m_Attenuation=1.f;
}

PointLight::~PointLight()
{

}

//===========================================================================//
// Constructeur PointLight						                             //
//===========================================================================//
SpotLight::SpotLight()
{

}

SpotLight::~SpotLight()
{

}
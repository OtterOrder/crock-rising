


//===========================================================================//
// Variables uniformes                                                       //
//===========================================================================//
float4x4 g_mWorld;                  // World matrix
float4x4 g_mWorldViewProjection;    // World * View * Projection matrix
matrix	 g_mWorldView;				// World * View matrix
matrix	 g_mView;					// View matrix

texture g_MeshTexture;              // Texture du mesh
bool    g_UseTex = false;
texture g_NormalMap;				// Texture de normal map
bool    g_UseNormalMap = false;
    
float4 g_ObjectAmbient;				// Propriétés de l'objet
float4 g_ObjectDiffuse;
float4 g_ObjectSpecular;
float  g_Glossiness;

static const int g_NumLights=1;				// Propriétés des lumières		   
float3 g_LightsPosition[4]; 
float3 g_LightsDirection[4];
float4 g_LightsColor[4]; 
float4 g_LightsSpecular[4]; 
float  g_LightsAttenuation[4];
float  g_LightsAngle[4];

float3 g_vCamPos;

//===========================================================================//
// Texture samplers	                                                         //
//===========================================================================//
sampler MeshTextureSampler = 
sampler_state
{
    Texture = <g_MeshTexture>;
    AddressU  = WRAP;        
	AddressV  = WRAP;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler NormalMapSampler = 
sampler_state
{
    Texture = <g_MeshTexture>;
    AddressU  = WRAP;        
	AddressV  = WRAP;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

//===========================================================================//
// Sortie vertex shader                                                      //
//===========================================================================//
struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position
    float2 TextureUV  : TEXCOORD0;  // vertex texture coords 
    float3 Normal	  : TEXCOORD1;
    float4 oPosition  : TEXCOORD2;
	float3 Tangent	  : TEXCOORD3;
	float3 Binormal	  : TEXCOORD4;
};

//===========================================================================//
// Ce shader calcule les transformations et éclairages standard              //
//===========================================================================//
VS_OUTPUT RenderSceneVS( float4 vPos : POSITION, 
                         float3 vNormal : NORMAL,
                         float2 vTexCoord0 : TEXCOORD0,
                         float3 vTangent : TANGENT,
                         float3 vBinormal : BINORMAL
                       )
{
    VS_OUTPUT Output;

    Output.Position = mul(vPos, g_mWorldViewProjection);
    Output.oPosition = mul(vPos, g_mWorld);
    Output.Normal = mul( vNormal, g_mWorld);

    Output.TextureUV = vTexCoord0;

    Output.Tangent = vTangent;
    Output.Binormal = vBinormal;

    return Output;    
}


//===========================================================================//
// Normal mapping                                                            //
//===========================================================================//

float3 GetBumpedNormal (float3 _Normal, float3 _Tangent, float3 Binormal, float2 _UV)
{
	float3 bumpedNormal;
	
	bumpedNormal = _Normal;	////

	float2 p = tex2D(NormalMapSampler, _UV);
	float px = tex2D(NormalMapSampler, _UV+float2(1.0f/256.0f, 0.0f)).x;
	float py = tex2D(NormalMapSampler, _UV+float2(0.0f, 1.0f/256.0f)).x;

	float2 bumps = 2.0f*float2(p.x-px, p.y-py);
	bumpedNormal = normalize( normalize(_Normal) + (bumps.x*_Tangent + bumps.y*Binormal));

	return bumpedNormal;
}


//===========================================================================//
// Sortie pixel shader                                                       //
//===========================================================================//
struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;  // Pixel color    
};

//===========================================================================//
// Illumination phong pixel shader                                           //
//===========================================================================//
PS_OUTPUT RenderScenePS( VS_OUTPUT In ) 
{ 
    PS_OUTPUT Output;
    
    float3 pos=In.oPosition.xyz/In.oPosition.w;
    
    float4 DiffuseColor;
    
    if(g_UseTex)
		DiffuseColor=tex2D(MeshTextureSampler, In.TextureUV.xy);
    else
		DiffuseColor=g_ObjectDiffuse;
		
	float3 V=normalize(g_vCamPos-pos);

	float3 N = g_UseNormalMap? normalize( GetBumpedNormal(In.Normal, normalize(In.Tangent), normalize(In.Binormal), In.TextureUV) ) : normalize(In.Normal);

	float att;
	float4 lumiere=float4(0.f, 0.f, 0.f, 0.f);
	float3 Color=float3(0.f, 0.f, 0.f);
	
	Output.RGBColor.xyz=float3(0.f, 0.f, 0.f);
		
	for(int i=0; i<g_NumLights; i++)
	{
		float3 L=normalize(g_LightsPosition[i]-pos);
		float3 dist = length(L);
		float3 H=normalize(L+V);
		
		float spotEffect = dot(normalize(g_LightsDirection[i]-g_LightsPosition[i]), -L);
		
		if(spotEffect > g_LightsAngle[i])
		{
			lumiere=lit(dot(N, L),dot(N, H), g_Glossiness);
			spotEffect = pow(spotEffect, 2);
			att = spotEffect / (g_LightsAttenuation[i] + 0.5f * dist + 0.5f * dist * dist);
		}
		else 
			att=1.f / (g_LightsAttenuation[i] + 0.5f * dist + 0.5f * dist * dist);
		
		Color += att*((g_ObjectAmbient+lumiere.y)*DiffuseColor+g_ObjectSpecular*lumiere.z);
	}
	
	Output.RGBColor.xyz=Color;
	Output.RGBColor.w=1.f;

	return Output;
}

//===========================================================================//
// Techniques					                                             //
//===========================================================================//

technique RenderScene
{
    pass P0
    {          
        VertexShader = compile vs_3_0 RenderSceneVS();
        PixelShader  = compile ps_3_0 RenderScenePS();
    }
}

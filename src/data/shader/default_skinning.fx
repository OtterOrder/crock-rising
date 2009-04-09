

//===========================================================================//
// Variables globales                                                        //
//===========================================================================//
float4x4 g_mWorld;                  // World matrix
float4x4 g_mWorldViewProjection;    // World * View * Projection matrix
matrix	 g_mWorldView;				// World * View matrix
matrix	 g_mView;					// View matrix
matrix	 g_mViewProj;				// View * Projection matrix

texture g_MeshTexture;              // Texture du mesh

float4 g_ObjectAmbient;				// Propriétés de l'objet
float4 g_ObjectDiffuse;
float4 g_ObjectSpecular;
bool   g_UseTex=false;
float  g_Glossiness;

static const int g_NumLights=1;				// Propriétés des lumières		   
float3 g_LightsPosition[4]; 
float4 g_LightsColor[4]; 
float4 g_LightsSpecular[4]; 
float  g_LightsAttenuation[4];
float  g_LightsAngle[4];

float3 g_vCamPos;

static const int MAX_MATRICES = 50;
float4x4	g_skinningMatrices[MAX_MATRICES];
bool		g_bShowBone=false;
bool		g_bTex=false;


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

//===========================================================================//
// Sortie vertex shader                                                      //
//===========================================================================//
struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position
    float2 TextureUV  : TEXCOORD0;  // vertex texture coords 
    float3 Normal	  : TEXCOORD1;
    float4 oPosition  : TEXCOORD2;
};

//===========================================================================//
// Ce shader calcule les transformations et éclairages standard              //
//===========================================================================//
VS_OUTPUT RenderSceneVS( float4 vPos : POSITION, 
                         float3 vNormal : NORMAL,
                         float2 vTexCoord0 : TEXCOORD0,
                         float4 vWeights : BLENDWEIGHT,
                         float4 vIndices : BLENDINDICES)
{
    VS_OUTPUT Output;
    
    // Calculate skinning transformations
    float3 skinnedPos  = (0.f).xxx;
    float3 skinnedNorm = (0.f).xxx;
    
    if(!g_bShowBone)
    {
	
		vWeights.w = 1.f - (vWeights.x + vWeights.y + vWeights.z);
				
		float4x4 skinningTransform = g_skinningMatrices[(int)vIndices[0]] * vWeights[0];
		skinningTransform += g_skinningMatrices[(int)vIndices[1]] * vWeights[1];
		skinningTransform += g_skinningMatrices[(int)vIndices[2]] * vWeights[2];
		skinningTransform += g_skinningMatrices[(int)vIndices[3]] * vWeights[3];

		//vPos = mul (vPos, g_mWorld);
		//vPos = mul (vPos, skinningTransform);
		
		vPos = mul(float4(vPos.xyz, 1.0f), skinningTransform);
		Output.Normal = mul( vNormal, skinningTransform);
		Output.Position =  mul(vPos, g_mViewProj);
		
		Output.oPosition = Output.Position;
    
    }
    else
    {
		Output.Position = mul(vPos, g_mWorldViewProjection);
		Output.Normal = vNormal;
    }
    
    
    Output.TextureUV = vTexCoord0;
    
    return Output;    
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
	float3 N=normalize(In.Normal.xyz);
	float att;
	float4 lumiere=float4(0.f, 0.f, 0.f, 0.f);
	float3 Color=float3(0.f, 0.f, 0.f);
	
	Output.RGBColor.xyz=float3(0.f, 0.f, 0.f);
		
	for(int i=0; i<g_NumLights; i++)
	{
		float3 L=normalize(g_LightsPosition[i]-pos);
		float3 dist = length(L);
		float3 H=normalize(L+V);
		
		float spotEffect = dot(normalize(g_LightsPosition[i]), normalize(L));
		
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
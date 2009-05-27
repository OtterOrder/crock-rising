


//===========================================================================//
// Variables uniformes                                                       //
//===========================================================================//
float4x4 g_mWorld;                  // World matrix
float4x4 g_mWorldViewProjection;    // World * View * Projection matrix
matrix	 g_mWorldView;				// World * View matrix
matrix	 g_mView;					// View matrix
matrix	 g_mViewProj;				// View * Projection matrix

texture g_MeshTexture;              // Texture du mesh
bool   g_UseTex=false;

texture g_NormalMapTexture;			// Texture de normal map
bool    g_UseNormalMap = false;

    
float4 g_ObjectAmbient;				// Propriétés de l'objet
float4 g_ObjectDiffuse;
float4 g_ObjectSpecular;
float  g_Glossiness;
float  g_Opacity;

static const int g_NumLights=1;		// Propriétés des lumières		   
float3 g_LightsPosition[4]; 
float3 g_LightsDirection[4];
float4 g_LightsColor[4]; 
float4 g_LightsSpecular[4]; 
float  g_LightsAttenuation[4];
float  g_LightsAngle[4];
float  g_LightsExponent[4];

float3 g_vCamPos;

static const int MAX_MATRICES = 50;
float4x4	g_skinningMatrices[MAX_MATRICES];

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
    Texture = <g_NormalMapTexture>;
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
	float3 Light : TEXCOORD2;
    float3 View : TEXCOORD3;
    float4 oPosition : TEXCOORD4;
};

//===========================================================================//
// Normal mapping                                                            //
//===========================================================================//

float3 GetBumpedNormal (float3 _Normal, float3 _Tangent, float3 _Binormal, float2 _UV)
{
	float3 bumpedNormal;
	
	bumpedNormal = _Normal;	////

	float2 p = tex2D(NormalMapSampler, _UV);
	float px = tex2D(NormalMapSampler, _UV+float2(1.0f/256.0f, 0.0f)).x;
	float py = tex2D(NormalMapSampler, _UV+float2(0.0f, 1.0f/256.0f)).x;

	float2 bumps = 1.0f*float2(p.x-px, p.y-py);
	bumpedNormal = normalize( normalize(_Normal) + (bumps.x*_Tangent + bumps.y*_Binormal));

	return bumpedNormal;
}

//===========================================================================//
// Ce shader calcule les transformations et éclairages standard              //
//===========================================================================//
VS_OUTPUT RenderSceneVS( float4 vPos : POSITION, 
                         float3 vNormal : NORMAL,
                         float2 vTexCoord0 : TEXCOORD0,
                         float3 vTangent : TANGENT,
                         float3 vBinormal : BINORMAL,
                         float4 vWeights : BLENDWEIGHT,
                         float4 vIndices : BLENDINDICES
                        )
{
     VS_OUTPUT Output;
    
    // Calculate skinning transformations
    float3 skinnedPos  = (0.f).xxx;
    float3 skinnedNorm = (0.f).xxx;
    

	vWeights.w = 1.f - (vWeights.x + vWeights.y + vWeights.z);
			
	float4x4 skinningTransform = g_skinningMatrices[(int)vIndices[0]] * vWeights[0];
	skinningTransform += g_skinningMatrices[(int)vIndices[1]] * vWeights[1];
	skinningTransform += g_skinningMatrices[(int)vIndices[2]] * vWeights[2];
	skinningTransform += g_skinningMatrices[(int)vIndices[3]] * vWeights[3];


	
	vPos = mul(float4(vPos.xyz, 1.0f), skinningTransform);
	Output.Normal = mul( vNormal, skinningTransform);
	Output.Position =  mul(vPos, g_mViewProj);
	
	Output.oPosition = Output.Position;
    
    float3x3 worldToTangentSpace;
    worldToTangentSpace[0] = mul(vTangent, skinningTransform);
    worldToTangentSpace[1] = mul(vBinormal, skinningTransform);
    worldToTangentSpace[2] = mul(vNormal, skinningTransform);
    
    Output.TextureUV = vTexCoord0;
   
   // float4 PosWorld = vPos;   
    
    float4 PosWorld = mul(vPos, skinningTransform);  

	float3 L=normalize(g_LightsPosition[0]-PosWorld);
    Output.Light = mul(worldToTangentSpace, L);							// L
    Output.View = mul(worldToTangentSpace, g_vCamPos - PosWorld);		// V
   
    
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
    
    float3 pos=In.oPosition.xyz;
    
    float4 DiffuseColor;
    float att,D=0, S=0;
	float4 lumiere=float4(0.f, 0.f, 0.f, 0.f);
	float3 Color=float3(0.f, 0.f, 0.f);
	int i=0;
    
    if(g_UseTex)
		DiffuseColor=tex2D(MeshTextureSampler, In.TextureUV.xy);
    else
		DiffuseColor=g_ObjectDiffuse;
  
	float3 V=normalize(In.View);	  // V
	
	float3 N =(2 * (tex2D(NormalMapSampler, In.TextureUV)))- 1.0;

	float3 LtoV=normalize(g_LightsPosition[0]-pos);
	float3 L=normalize(In.Light); 
	float dist = length(normalize(LtoV));
	float3 H=normalize(L+V);
	
	float spotEffect = dot(normalize(g_LightsDirection[i]-g_LightsPosition[i]), -LtoV);
	
	if(spotEffect > g_LightsAngle[i])
	{
		D = saturate(dot(N, L));
		float3 R = normalize(2 * D * N - L);
		S = min(pow(saturate(dot(R, V)), g_Glossiness), DiffuseColor.w);
		spotEffect = pow(spotEffect, g_LightsExponent[i]);
		att = spotEffect / (g_LightsAttenuation[i] + 0.5f * dist + 0.5f * dist * dist);
	}
	else 
		att=1.f / (g_LightsAttenuation[i] + 0.5f * dist + 0.5f * dist * dist);
	
	Color += att*(g_ObjectAmbient + D*(DiffuseColor+S*g_ObjectSpecular));

    
	Output.RGBColor.xyz=Color;
	Output.RGBColor.a=g_Opacity;
	
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

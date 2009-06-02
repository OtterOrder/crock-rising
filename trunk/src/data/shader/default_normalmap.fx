


//===========================================================================//
// Variables uniformes                                                       //
//===========================================================================//
float4x4 g_mWorld;                  // World matrix
float4x4 g_mWorldViewProjection;    // World * View * Projection matrix
matrix	 g_mWorldView;				// World * View matrix
matrix	 g_mView;					// View matrix

texture g_MeshTexture;              // Texture du mesh
bool   g_UseTex=false;

texture g_TexShadowMap;
matrix  g_mTexProj;
matrix  g_mLightViewProj;			// Données ShadowMap

texture g_NormalMapTexture;			// Texture de normal map
    
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

sampler ShadowTextureSampler = 
sampler_state
{
    Texture = <g_TexShadowMap>;
    AddressU  = BORDER;        
	AddressV  = BORDER;
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
	float3 Light : TEXCOORD1;
    float3 View : TEXCOORD2;
    float4 oPosition : TEXCOORD3;
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
                         float3 vBinormal : BINORMAL
                        )
{
    VS_OUTPUT Output;
    
    Output.Position = mul(vPos, g_mWorldViewProjection);
    Output.oPosition = mul(vPos, g_mWorld);
    
    float3x3 worldToTangentSpace;
    worldToTangentSpace[0] = mul(vTangent, g_mWorld);
    worldToTangentSpace[1] = mul(vBinormal, g_mWorld);
    worldToTangentSpace[2] = mul(vNormal, g_mWorld);
    
    Output.TextureUV = vTexCoord0;
   
    float4 PosWorld = mul(vPos, g_mWorld);   
   
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




//===========================================================================//
//							SHADOW MAP                                       //
//===========================================================================//









//===========================================================================//
// Sortie vertex shader                                                      //
//===========================================================================//
struct VS_OUTPUTS
{
    float4 Position   : POSITION;   // vertex position
    float2 TextureUV  : TEXCOORD0;  // vertex texture coords 
	float3 Light	  : TEXCOORD1;
    float3 View		  : TEXCOORD2;
    float4 oPosition  : TEXCOORD3;
    float4 ProjUV	  : TEXCOORD4;
    float  fDepth     : TEXCOORD5;
};

//===========================================================================//
// Ce shader calcule les transformations et éclairages standard              //
//===========================================================================//
VS_OUTPUTS RenderSceneShadowVS( float4 vPos : POSITION, 
                         float3 vNormal : NORMAL,
                         float2 vTexCoord0 : TEXCOORD0,
                         float3 vTangent : TANGENT,
                         float3 vBinormal : BINORMAL)
{
	VS_OUTPUTS Output;
    
    Output.Position = mul(vPos, g_mWorldViewProjection);
    Output.oPosition = mul(vPos, g_mWorld);
    
    float3x3 worldToTangentSpace;
    worldToTangentSpace[0] = mul(vTangent, g_mWorld);
    worldToTangentSpace[1] = mul(vBinormal, g_mWorld);
    worldToTangentSpace[2] = mul(vNormal, g_mWorld);
    
    Output.TextureUV = vTexCoord0;
   
    float4 PosWorld = mul(vPos, g_mWorld);   
   
	float3 L=normalize(g_LightsPosition[0]-PosWorld);
    Output.Light = mul(worldToTangentSpace, L);							// L
    Output.View = mul(worldToTangentSpace, g_vCamPos - PosWorld);		// V
    
    // Output the projective texture coordinates
	Output.ProjUV = mul( vPos, g_mTexProj );
	Output.fDepth = mul( vPos, g_mLightViewProj ).z;
    
    return Output;    
}


//===========================================================================//
// Sortie pixel shader                                                       //
//===========================================================================//
struct PS_OUTPUTS
{
    float4 RGBColor : COLOR0;  // Pixel color    
};

//===========================================================================//
// Illumination phong pixel shader                                           //
//===========================================================================//
PS_OUTPUTS RenderSceneShadowPS( VS_OUTPUTS In ) 
{ 
	PS_OUTPUTS Output;
    
    float3 pos=In.oPosition.xyz;
    
    float att,D=0, S=0;
    float4 DiffuseColor;
	
	// On génère les 9 coordonnées de textures dans un kernel 3x3
	float4 vTexCoords[9];
	// Taille d'un texel
	float fTexelSize = 1.0f / 512.0f;

	// Génération des coordonnées de textures
	// 4 3 5
	// 1 0 2
	// 7 6 8
	vTexCoords[0] = In.ProjUV;
	vTexCoords[1] = In.ProjUV + float4( -fTexelSize, 0.0f, 0.0f, 0.0f );
	vTexCoords[2] = In.ProjUV + float4(  fTexelSize, 0.0f, 0.0f, 0.0f );
	vTexCoords[3] = In.ProjUV + float4( 0.0f, -fTexelSize, 0.0f, 0.0f );
	vTexCoords[6] = In.ProjUV + float4( 0.0f,  fTexelSize, 0.0f, 0.0f );
	vTexCoords[4] = In.ProjUV + float4( -fTexelSize, -fTexelSize, 0.0f, 0.0f );
	vTexCoords[5] = In.ProjUV + float4(  fTexelSize, -fTexelSize, 0.0f, 0.0f );
	vTexCoords[7] = In.ProjUV + float4( -fTexelSize,  fTexelSize, 0.0f, 0.0f );
	vTexCoords[8] = In.ProjUV + float4(  fTexelSize,  fTexelSize, 0.0f, 0.0f );
	// Test pour savoir si le pixel est dans l'ombre
	float fShadowTerms[9];
	float fShadowTerm = 0.0f;
	int i;
	for(  i = 0; i < 9; i++ )
	{
		float A = tex2Dproj( ShadowTextureSampler, vTexCoords[i] ).r;
		float B = (In.fDepth - 5.f);

		// Le texel est ombré
		fShadowTerms[i] = A < B ? 0.5f : 1.f;
		fShadowTerm     += fShadowTerms[i];
	}
	// On fait la moyenne
	fShadowTerm /= 9.0f;
    
    float4 lumiere=float4(0.f, 0.f, 0.f, 0.f);
	float3 Color=float3(0.f, 0.f, 0.f);
	
	i=0;
	
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
		D = saturate(dot(N, L))*fShadowTerm;
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

technique RenderSceneShadow
{
    pass P0
    {      
        VertexShader = compile vs_3_0 RenderSceneShadowVS();
        PixelShader  = compile ps_3_0 RenderSceneShadowPS();
    }
}


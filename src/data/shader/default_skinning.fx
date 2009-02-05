

//===========================================================================//
// Variables globales                                                        //
//===========================================================================//
float4x4 g_mWorld;                  // World matrix
float4x4 g_mWorldViewProjection;    // World * View * Projection matrix
matrix	 g_mWorldView;				// World * View matrix
matrix	 g_mView;					// View matrix
matrix	 g_mViewProj;				// View * Projection matrix

texture g_MeshTexture;              // Texture du mesh

float4 g_MaterialAmbientColor=float4(0.5f, 0.5f, 0.5f, 1.0f);      
float4 g_MaterialDiffuseColor=float4(0.6f, 0.6f, 0.6f, 1.0f);  
float4 g_LightDiffuse=float4(0.5f, 0.5f, 0.5f, 1.0f);
float3 g_LightDir=float3(0.0f, 0.0f, 0.0f);
float4 g_LightAmbient=float4(0.8f, 0.8f, 0.7f, 1.0f);
float4 g_MaterialSpecularColor=float4(0.5f, 0.5f, 0.5f, 1.0f);
float  g_MaterialSpecularPower=8.0f;

float3 g_vLightPos=float3(0.0f, 50.0f, 0.0f );
float3 g_vCamPos;

static const int MAX_MATRICES = 30;
float4x4	g_skinningMatrices[MAX_MATRICES];

float4x4	g_mIdentity;


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
    float4 Test		  : TEXCOORD3;	//// TEMP
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
	
	vWeights.w = 1.f - (vWeights.x + vWeights.y + vWeights.z);
	
	int id = 0;
	//Output.Test = float4(vIndices.xyz, 1.f);
	//Output.Test = ((int)vIndices[id] == 0)? float4(1.f, 0.f, 0.f, 1.f) : float4(0.f, 0.f, 0.f, 1.f);
	Output.Test = ((int)vIndices[id] >= 22)? float4(1.f, 0.f, 0.f, 1.f) : ((int)vIndices[id] < 0)? float4(0.f, 0.f, 1.f, 1.f) : float4(0.f, 0.f, 0.f, 1.f);
	
	float4x4	skinningTransform  = g_skinningMatrices[(int)vIndices[0]] ;//* vWeights[0];
	/*
				skinningTransform += g_skinningMatrices[(int)vIndices[1]] * vWeights[1];
				skinningTransform += g_skinningMatrices[(int)vIndices[2]] * vWeights[2];
				skinningTransform += g_skinningMatrices[(int)vIndices[2]] * vWeights[3];
	//*/
	
	//skinningTransform = g_mIdentity;

	//vPos = mul (vPos, skinningTransform);
	
	Output.Position = mul(vPos, g_mWorldViewProjection);
    Output.oPosition = Output.Position;
    Output.Normal = (mul( vNormal, g_mWorldView ));
    
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
    
    float4 lightInView= mul( g_vLightPos, g_mView);
    
    // Vecteurs utiles pour calculer la lumière
    float3 L = normalize(lightInView.xyz-In.oPosition);
    float3 N = normalize(In.Normal);
    float3 V = normalize(g_vCamPos-In.oPosition);
    float3 H = normalize(L+V);
	
	float specularTerm =  pow(max(0.0f, dot(N, H)), g_MaterialSpecularPower );
	
	//Réflexion diffuse
	float3 vLightDiffuse=g_LightDiffuse * max(0,dot(N, L));
	
	//Réflexion spéculaire
	float4 SpecResult=saturate(g_MaterialSpecularColor*specularTerm);
    
    //Couleur finale (ambient+diffuse+specular)
    float4 FinalIllumination = SpecResult+g_MaterialDiffuseColor*float4(vLightDiffuse, 1.0f)+g_MaterialAmbientColor*g_LightAmbient;
    Output.RGBColor.xyz = tex2D(MeshTextureSampler, In.TextureUV.xy)* FinalIllumination; 
    Output.RGBColor.a = 1.0f;
    
    //Output.RGBColor = In.Weights;
    //Output.RGBColor.a = 1.0f;

    return Output;
}

//===========================================================================//
// Illumination phong pixel shader                                           //
//===========================================================================//
PS_OUTPUT RenderScenePSNoTex( VS_OUTPUT In ) 
{ 
    PS_OUTPUT Output;
    
    float4 lightInView= mul( g_vLightPos, g_mView);
    
    // Vecteurs utiles pour calculer la lumière
    float3 L = normalize(lightInView.xyz-In.oPosition);
    float3 N = normalize(In.Normal);
    float3 V = normalize(g_vCamPos-In.oPosition);
    float3 H = normalize(L+V);
    
    float specularTerm =  pow(max(0.0f, dot(N, H)), g_MaterialSpecularPower );
	
	//Réflexion diffuse
	float3 vLightDiffuse=g_LightDiffuse * max(0,dot(N, L));
	
	//Réflexion spéculaire
	float4 SpecResult=saturate(g_MaterialSpecularColor*specularTerm);
    
    //Couleur finale (ambient+diffuse+specular)
    float4 FinalIllumination = /*SpecResult+*/g_MaterialDiffuseColor*float4(vLightDiffuse, 1.0f)+g_MaterialAmbientColor*g_LightAmbient;
    Output.RGBColor.xyz = FinalIllumination; 
    Output.RGBColor.a = 1.0f;
    
    Output.RGBColor += In.Test;
    Output.RGBColor.a = 1.0f;

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

technique RenderSceneNoTex
{
    pass P0
    {          
        VertexShader = compile vs_3_0 RenderSceneVS();
        PixelShader  = compile ps_3_0 RenderScenePSNoTex();
    }
}


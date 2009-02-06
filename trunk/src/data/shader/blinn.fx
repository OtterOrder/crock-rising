


//===========================================================================//
// Variables globales                                                        //
//===========================================================================//
float4x4 g_mWorld;                  // World matrix
float4x4 g_mWorldViewProjection;    // World * View * Projection matrix
matrix	 g_mWorldView;				// World * View matrix
matrix	 g_mView;					// View matrix

texture g_MeshTexture;              // Texture du mesh

float4 g_MaterialAmbientColor=float4(0.5f, 0.5f, 0.5f, 1.0f);      
float4 g_MaterialDiffuseColor=float4(0.0f, 1.0f, 0.0f, 1.0f);  
float4 g_LightDiffuse=float4(0.5f, 0.5f, 0.5f, 1.0f);
float3 g_LightDir=float3(0.0f, 0.0f, 0.0f);
float4 g_LightAmbient=float4(0.8f, 0.8f, 0.7f, 1.0f);
float4 g_MaterialSpecularColor=float4(1.0f, 1.0f, 1.0f, 1.0f);
float  g_MaterialSpecularPower=8.0f;

float3 g_vLightPos=float3(0.0f, 100.0f, 0.0f );
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
                         float2 vTexCoord0 : TEXCOORD0)
{
    VS_OUTPUT Output;
    
    Output.Position = mul(vPos, g_mWorldViewProjection);
    Output.oPosition = mul(vPos, g_mWorld);
    Output.Normal = mul( vNormal, g_mWorld);
    
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
  
	float3 L=normalize(g_vLightPos.xyz-pos);
	float3 V=normalize(g_vCamPos-pos);
	float3 H=normalize(L+V);
	float3 N=normalize(In.Normal.xyz);
	
	float4 lumiere=lit(dot(N, L),dot(N, H), 30);
  
    Output.RGBColor=g_MaterialDiffuseColor*lumiere.y+g_LightDiffuse*lumiere.z;
    
    //Output.RGBColor.xyz = tex2D(MeshTextureSampler, In.TextureUV.xy)* FinalIllumination; 

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

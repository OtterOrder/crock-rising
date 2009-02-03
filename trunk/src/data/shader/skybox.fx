

//===========================================================================//
// Variables globales                                                        //
//===========================================================================//
float4x4 g_mWorldViewProjection;    // World * View * Projection matrix
float4x4 g_mWorld;                  // World matrix

texture g_txEnvMap;

float3 g_vCamPos;

//===========================================================================//
// Texture samplers	                                                         //
//===========================================================================//
samplerCUBE g_samEnvMap =
sampler_state
{
    Texture = <g_txEnvMap>;
    AddressU = Mirror;
    AddressV = Mirror;
    magfilter = LINEAR; 
    minfilter = LINEAR; 
    mipfilter = LINEAR; 

};

//===========================================================================//
// Sortie vertex shader                                                      //
//===========================================================================//
struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position 
    float3 ViewDirection : TEXCOORD2;
};

//===========================================================================//
// Ce shader calcule les transformations et éclairages standard              //
//===========================================================================//
VS_OUTPUT RenderSceneVS( float4 vPos : POSITION)
{
    VS_OUTPUT Output;
    
    Output.Position = mul(vPos, g_mWorldViewProjection);
    float3 ObjectPosition = mul(vPos, g_mWorld);
    
    Output.ViewDirection = ObjectPosition-g_vCamPos;	
    
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

	float3 ViewDirection = normalize(In.ViewDirection);
	ViewDirection.x *= -1;
	Output.RGBColor=texCUBE( g_samEnvMap, ViewDirection);

    return Output;
}

//===========================================================================//
// Techniques					                                             //
//===========================================================================//

technique RenderSkybox
{
    pass P0
    {          
        VertexShader = compile vs_3_0 RenderSceneVS();
        PixelShader  = compile ps_3_0 RenderScenePS();
    }
}
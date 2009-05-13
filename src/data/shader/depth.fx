


//===========================================================================//
// Variables uniformes                                                       //
//===========================================================================//
matrix	g_matLightViewProj	: LightViewProjection;

//===========================================================================//
// Sortie vertex shader                                                      //
//===========================================================================//
struct VSOUTPUT_SHADOW
{
   float4 vPosition    : POSITION;
   float  fDepth       : TEXCOORD0;
};

//===========================================================================//
// Ce shader calcule les transformations et éclairages standard              //
//===========================================================================//
VSOUTPUT_SHADOW VS_Shadow( float4 inPosition : POSITION )
{
   // Output struct
   VSOUTPUT_SHADOW OUT = (VSOUTPUT_SHADOW)0;
   // Output the transformed position
   OUT.vPosition = mul( inPosition, g_matLightViewProj );
   // Output the scene depth
   OUT.fDepth = OUT.vPosition.z;
   return OUT;
}

float4  PS_Shadow( VSOUTPUT_SHADOW IN ) : COLOR0
{
   // Output the scene depth
   return float4( IN.fDepth, IN.fDepth, IN.fDepth, 1.0f );
}


//===========================================================================//
// Techniques					                                             //
//===========================================================================//

technique RenderShadow
{
    pass P0
    { 
		Lighting	= False;
		CullMode	= CCW;
		         
        VertexShader = compile vs_3_0 VS_Shadow();
        PixelShader  = compile ps_3_0 PS_Shadow();
    }
}

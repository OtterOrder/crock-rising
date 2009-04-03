//===========================================================================//
// Variables uniformes                                                       //
//===========================================================================//
float4x4 g_mWorldViewProjection;    // World * View * Projection matrix
float4x4 g_mWorldViewProjLast;		// Last WorldViewProjection matrix

//===========================================================================//
// Sortie vertex shader                                                      //
//===========================================================================//
struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position
    float2 oPosition  : TEXCOORD0;  // vertex in
};

//===========================================================================//
// Ce shader calcule les transformations et éclairages standard              //
//===========================================================================//
VS_OUTPUT VSVelocity( float4 vPos : POSITION )
{
    VS_OUTPUT Output;

    Output.Position = mul(vPos, g_mWorldViewProjection);

    Output.oPosition = vPos;

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
// Velocity				                                                     //
//===========================================================================//
float2 ComputeVelocity (float4 _position)
{
	float4 vPosProjSpaceCurrent	= mul(_position, g_mWorldViewProjection);
	float4 vPosProjSpaceLast	= mul(_position, g_mWorldViewProjLast);

	// Convert to non-homogeneous points [-1,1] by dividing by w 
	vPosProjSpaceCurrent /= vPosProjSpaceCurrent.w;
	vPosProjSpaceLast /= vPosProjSpaceLast.w;

	float2 velocity = vPosProjSpaceCurrent - vPosProjSpaceLast;    

	// The velocity is now between (-2,2) so divide by 2 to get it to (-1,1)
	velocity /= 2.0f;
	
	return velocity;
}

PS_OUTPUT PSVelocity( VS_OUTPUT In ) 
{ 
    PS_OUTPUT Output;
	
	 Output.RGBColor = float4( (ComputeVelocity(In.oPosition) +1.f)/2.f, 0.f, 1.f);

	return Output;
}
//===========================================================================//
// Techniques					                                             //
//===========================================================================//

technique RenderVelocity
{
    pass P0
    {          
        VertexShader = compile vs_3_0 VSVelocity();
        PixelShader  = compile ps_3_0 PSVelocity();
    }
}
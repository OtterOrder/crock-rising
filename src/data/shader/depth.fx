


//===========================================================================//
// Variables uniformes                                                       //
//===========================================================================//
float4x4	g_mWorldView;
float4x4    g_mViewProj;
float4x4    g_mWorld;

static const int MAX_MATRICES = 50;
float4x4	g_skinningMatrices[MAX_MATRICES];

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
   inPosition = mul(float4(inPosition.xyz, 1.0f), g_mWorld);
   OUT.vPosition = mul( inPosition, g_mViewProj );
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
		//Fillmode=Wireframe;     
        VertexShader = compile vs_3_0 VS_Shadow();
        PixelShader  = compile ps_3_0 PS_Shadow();
    }
}



//===========================================================================//
//					         Animated depth                                  //
//===========================================================================//






//===========================================================================//
// Ce shader calcule les transformations et éclairages standard              //
//===========================================================================//
VSOUTPUT_SHADOW VS_ShadowA( float4 vPos : POSITION,
						   float4 vWeights : BLENDWEIGHT,
                           float4 vIndices : BLENDINDICES )
{
   // Output struct
   VSOUTPUT_SHADOW OUT = (VSOUTPUT_SHADOW)0;
   
   
   vWeights.w = 1.f - (vWeights.x + vWeights.y + vWeights.z);
				
   float4x4 skinningTransform = g_skinningMatrices[(int)vIndices[0]] * vWeights[0];
   skinningTransform += g_skinningMatrices[(int)vIndices[1]] * vWeights[1];
   skinningTransform += g_skinningMatrices[(int)vIndices[2]] * vWeights[2];
   skinningTransform += g_skinningMatrices[(int)vIndices[3]] * vWeights[3];

   vPos = mul(float4(vPos.xyz, 1.0f), skinningTransform);
   OUT.vPosition =  mul(vPos, g_mViewProj);


   // Output the scene depth
   OUT.fDepth = OUT.vPosition.z;
   return OUT;
}


//===========================================================================//
// Techniques					                                             //
//===========================================================================//

technique RenderShadowAnimated
{
    pass P0
    { 		 
		//Fillmode=Wireframe;     
        VertexShader = compile vs_3_0 VS_ShadowA();
        PixelShader  = compile ps_3_0 PS_Shadow();
    }
}


//===========================================================================//
// Variables uniformes                                                       //
//===========================================================================//
texture g_TextureToBlur;
texture g_VectorsTexture;

float g_BlurFactor = 1.f;
static const float NbSamples = 6.0f;

//===========================================================================//
// Texture samplers	                                                         //
//===========================================================================//
sampler TextureToBlur = 
sampler_state
{
    Texture = <g_TextureToBlur>;
    AddressU  = CLAMP;        
	AddressV  = CLAMP;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler VectorsTexture = 
sampler_state
{
    Texture = <g_VectorsTexture>;
    AddressU  = CLAMP;        
	AddressV  = CLAMP;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

//===========================================================================//
// Sortie pixel shader                                                       //
//===========================================================================//
struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;  // Pixel color
};

PS_OUTPUT PSVectorialBlur( float2 UV: TEXCOORD0 )
{ 
	PS_OUTPUT Output;
	
	Output.RGBColor = float4(1.f, 0.f, 0.f, 1.f);
	 
	float2 Vector;
	 
	float4 pixelVector = tex2D(VectorsTexture, UV);
	//pixelVector = pixelVector*2.f -1.f;

	Vector.x =  pixelVector.r * g_BlurFactor * (pixelVector.z*2.f -1.f);
	Vector.y = -pixelVector.g * g_BlurFactor * (pixelVector.w*2.f -1.f);

	float3 Blurred = 0;
	for(float i = 0; i < NbSamples; i++)
	{   
		float2 textureUV = Vector * i / NbSamples + UV;

		float4 Current = tex2D(TextureToBlur, textureUV);

		// Add it with the other samples
		Blurred += Current.rgb;
	}
	
	Vector = -Vector;
	for(float i = 0; i < NbSamples; i++)
	{   
		float2 textureUV = Vector * i / NbSamples + UV;

		float4 Current = tex2D(TextureToBlur, textureUV);

		// Add it with the other samples
		Blurred += Current.rgb;
	}
    
    // Return the average color of all the samples
    Output.RGBColor = float4(Blurred / (NbSamples*2.f), 1.0f);
    
    //// Delete Me !!!!
    //Output.RGBColor = pixelVector;//tex2D(VectorsTexture, UV);

	return Output;
}
//===========================================================================//
// Techniques					                                             //
//===========================================================================//

technique VectorialBlur
{
    pass P0
    {
        PixelShader  = compile ps_3_0 PSVectorialBlur();
    }
}
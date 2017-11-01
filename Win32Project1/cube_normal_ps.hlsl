
/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer : register(c0)
{
    float4 diffuseColor;
    float3 lightDirection;
    float LightBufferpadding;
};

cbuffer AlphaBlendBuffer : register(c1)
{
	float alpha;
	float3 BlendBufferPadding;
	
};

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;
    float4 ambientColor = float4(0.15f, 0.15f, 0.15f, 1.0f);
    float4 diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);
    
    color = ambientColor;

    // Invert the light direction for calculations.
    lightDir = -lightDirection;

    // Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, lightDir));
   
    if (lightIntensity > 0.0f)
    {
        // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        color += (diffuse * lightIntensity);
    }

     // Saturate the final light color.
    color = saturate(color);

    // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    //color = color * textureColor;
    color =textureColor;
	color.a = alpha;

    return color;
}
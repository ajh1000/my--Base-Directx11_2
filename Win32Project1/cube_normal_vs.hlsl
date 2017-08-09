
/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer:register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer boneTransformsBuffer : register(b1)
{
	matrix boneTransform[96];
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

struct SkinnedVertexInputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float4 weights : WEIGHTS;
    uint4 boneID : BONEINDICES;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(SkinnedVertexInputType input)
{
   
    PixelInputType output;

    
    float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    weights[0] = input.weights.x;
    weights[1] = input.weights.y;
    weights[2] = input.weights.z;
    weights[3] = 1.0f - (weights[0] +weights[1] + weights[2]);

    float3 posL = float3(0.0f, 0.0f, 0.0f);
    float3 normalL = float3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < 4; ++i)
    {
	    // Assume no nonuniform scaling when transforming normals, so 
		// that we do not have to use the inverse-transpose.
        posL += weights[i] * mul(float4(input.position, 1.0f), boneTransform[input.boneID[i]]).xyz;
        normalL += weights[i] * mul(input.normal, (float3x3) boneTransform[input.boneID[i]]);
    }
 

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(float4(posL, 1.0), worldMatrix);
    //output.position = mul(float4(input.position, 1.0f), worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
  

    // Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3) worldMatrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

    return output;
}

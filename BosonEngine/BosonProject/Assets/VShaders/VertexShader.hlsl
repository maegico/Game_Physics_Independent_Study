cbuffer externalData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};

struct VertexShaderInput
{
	float3 position		: POSITION;     // XYZ position
	float3 normal		: NORMAL;       // XYZ normal
	float2 uv			: TEXTCOORD;	// XY texture coordinate
};

struct VertexToPixel
{
	float4 position		: SV_POSITION;     // XYZ position
};

VertexToPixel main(VertexShaderInput input)
{
	VertexToPixel output;

	matrix worldViewProj = mul(mul(world, view), projection);
	output.position = mul(float4(input.position, 1.0f), worldViewProj);

	return output;
}
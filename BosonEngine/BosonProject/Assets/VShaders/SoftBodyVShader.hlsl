cbuffer externalData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};

cbuffer softbodyData : register(b1)
{
	float3 deformationVector;
	float3 collisionPoint;
};

//need to actually figure out what will go into a soft body interaction
//then need to update the position accordingly

//f = -kx
//f = ma
//ma/-k = x  <- total deformation??, would this work by itself as the other objects acceleration decreases

//More advanced and more correct form
//sum of f = ma
//fa + fb = ma = -kx
//use this to determine the true acceleration

//needs to pass up ma/-k
//need to pass up collision point
	//which can be found by finding the vector between spheres and normalizing it and then scaling it by the radius and adding it to the position

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

	//since we store the collision point in world space
	float distance = length(output.position - collisionPoint);
	//try a function like (x+2/x^2), x being the distance
	//need to be careful with the fact that:
		//the deformation is increased scaled for distance < .5 about
	distance = (distance + 1) / (distance * distance * 10);
	float3 scaleDeformVec = deformationVector * distance;

	output.position += float4(scaleDeformVec, 0.0f);

	return output;
}
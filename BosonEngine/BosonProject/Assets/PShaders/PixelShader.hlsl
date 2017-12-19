struct VertexToPixel
{
	float4 position		: SV_POSITION;     // XYZ position
};

float4 main(VertexToPixel input) : SV_TARGET
{
	//return float4(1.0f, 0.0f, 0.0f, 1.0f);
	return float4(input.position.x/500, input.position.x / 500, input.position.x / 500, 1);
}
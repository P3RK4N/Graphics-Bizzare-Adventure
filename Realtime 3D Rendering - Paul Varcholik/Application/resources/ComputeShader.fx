RWTexture2D<float4> OutputTexture;

cbuffer CBufferPerFrame
{
	float2 TextureSize;
	float BlueColor;
};

[numthreads(32, 32, 1)]
void compute_shader( uint3 threadID : SV_DispatchThreadID )
{
	float4 color = float4((threadID.xy / TextureSize), BlueColor, 1);
	OutputTexture[threadID.xy] = color;
}

technique11 compute
{
	pass p0
	{
		SetVertexShader(NULL);
		SetGeometryShader(NULL);
		SetHullShader(NULL);
		SetDomainShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, compute_shader()));
	}
};
cbuffer CBufferPerObject
{
	float4x4 WorldViewProjection :	WORLDVIEWPROJECTION;
}

struct VS_INPUT
{
	 float4 ObjectPosition: POSITION;
	 float2 TextureCoordinate : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 Position: SV_Position;
	float2 TextureCoordinate : TEXCOORD;
};

RasterizerState DisableCulling
{
	CullMode = NONE;
};

SamplerState ColorSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

Texture2D ColorTexture;

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
	 VS_OUTPUT OUT = (VS_OUTPUT)0;
	 OUT.Position = mul(IN.ObjectPosition, WorldViewProjection);
	 OUT.TextureCoordinate = IN.TextureCoordinate;
	 return OUT;
}

float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
	 return ColorTexture.Sample(ColorSampler, IN.TextureCoordinate);
}

technique11 main11
{
	 pass p0
	 {
		 SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
		 SetGeometryShader(NULL);
		 SetPixelShader(CompileShader(ps_5_0, pixel_shader()));
		 SetRasterizerState(DisableCulling);
	 }
}
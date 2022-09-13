cbuffer CBufferPerObject
{
	float4x4 WorldViewProjection :	WORLDVIEWPROJECTION;
	float4x4 World : WORLD;
}

cbuffer CBufferPerFrame
{
	float3 CameraPosition;

	float4 AmbientColor;
	float3 SpecularColor;
	float SpecularPower;
	
	float4 LightColor;
	float3 LightDirection;
	float3 LightPosition;
	float LightRadius;
};

struct VS_INPUT
{
	 float4 ObjectPosition : POSITION;
	 float2 TextureCoordinate : TEXCOORD;
	 float3 Normal : NORMAL;
};

struct VS_OUTPUT
{
	float4 Position: SV_Position;
	float2 TextureCoordinate : TEXCOORD;
	float3 Normal : NORMAL;
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
	 OUT.Normal = mul(IN.Normal, World);
	 return OUT;
}

float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
	float3 normal = normalize(IN.Normal);
	float3 lightDir = normalize(LightDirection);
	float ndotl = dot(normal, lightDir);

	float4 color = ColorTexture.Sample(ColorSampler, IN.TextureCoordinate);
	float3 ambient = AmbientColor.rgb * AmbientColor.a * color.rgb;

	float3 diffuse = (float3)0;
	if(ndotl > 0)
	{
		diffuse = LightColor.rgb * LightColor.a * ndotl * color.rgb;
	}

	return float4(ambient + diffuse, color.a);
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

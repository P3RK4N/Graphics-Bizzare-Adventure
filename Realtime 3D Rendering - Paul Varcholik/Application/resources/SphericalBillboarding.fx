#define SUBDIVISION 100.0f

static const float2 QuadUV[4] =
{
	float2(0.0f, 1.0f),
	float2(0.0f, 0.0f),
	float2(1.0f, 0.0f),
	float2(1.0f, 1.0f)
};

cbuffer CBufferPerFrame
{
	float3 CameraPosition : CAMERAPOSITION;
	float3 CameraUp;
	float TessellationEdgeFactors[4] = { SUBDIVISION, SUBDIVISION, SUBDIVISION, SUBDIVISION };
	float TessellationInsideFactors[2] = { SUBDIVISION, SUBDIVISION };
};

cbuffer CBufferPerObject
{
	float4x4 ViewProjection;
	float4x4 TextureMatrix;
	float SubdivisionFactor = 1.0f;
	float SubdivisionStrength = 0.0f;
};

Texture2D ColorTexture;

SamplerState ColorSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

RasterizerState Wireframe
{
	FillMode = WIREFRAME;
	CullMode = NONE;
};

struct VS_INPUT
{
	float4 Position : POSITION;
	float2 TextureCoordinate : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 Position : POSITION;
	float2 TextureCoordinate : TEXCOORD;
};

struct VS_OUTPUT_COLOR
{
	float4 Position : SV_Position;
	float2 TextureCoordinate : TEXCOORD;
};

struct HS_CONSTANT_OUTPUT
{
	float EdgeFactors[4] : SV_TessFactor;
	float InsideFactors[2] : SV_InsideTessFactor;
};

struct HS_OUTPUT
{
	float4 ObjectPosition : POSITION;
	float2 TextureCoordinate : TEXCOORD;
};

struct DS_OUTPUT
{
	float4 Position : SV_Position;
	float2 TextureCoordinate : TEXCOORD;
};

struct GS_OUTPUT
{
	float4 Position : SV_Position;
	float2 TextureCoordinate : TEXCOORD;
};



VS_OUTPUT vertex_shader(VS_INPUT IN)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;

	OUT.Position = IN.Position;
	OUT.TextureCoordinate = mul(float4(IN.TextureCoordinate * 0.3f,0,1), TextureMatrix).xy;

	return OUT;
}

//VS_OUTPUT_COLOR vertex_shader_color(VS_INPUT IN)
//{
//	VS_OUTPUT_COLOR OUT = (VS_OUTPUT_COLOR)0;
//
//	OUT.Position = mul(IN.Position, ViewProjection);
//	OUT.Size = IN.Size;
//
//	return OUT;
//}



HS_CONSTANT_OUTPUT constant_hull_shader(InputPatch<VS_OUTPUT, 4> patch, uint patchID : SV_PrimitiveID)
{
	HS_CONSTANT_OUTPUT OUT = (HS_CONSTANT_OUTPUT)0;

	[unroll]
	for(int i = 0; i < 4; i++)
	{
		//OUT.EdgeFactors[i] = TessellationEdgeFactors[i];
		OUT.EdgeFactors[i] = SubdivisionFactor;
	}

	/*OUT.InsideFactors[0] = TessellationInsideFactors[0];
	OUT.InsideFactors[1] = TessellationInsideFactors[1];*/

	OUT.InsideFactors[0] = SubdivisionFactor;
	OUT.InsideFactors[1] = SubdivisionFactor;

	return OUT;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("constant_hull_shader")]
HS_OUTPUT hull_shader(InputPatch<VS_OUTPUT, 4> patch, uint controlPointID : SV_OutputControlPointID)
{
	HS_OUTPUT OUT = (HS_OUTPUT)0;

	OUT.ObjectPosition = patch[controlPointID].Position;
	OUT.TextureCoordinate = patch[controlPointID].TextureCoordinate;

	return OUT;
}

[domain("quad")]
DS_OUTPUT domain_shader(HS_CONSTANT_OUTPUT IN, float2 uv : SV_DomainLocation, const OutputPatch<HS_OUTPUT, 4> patch)
{
	DS_OUTPUT OUT = (DS_OUTPUT)0;

	float4 v0 = lerp(patch[0].ObjectPosition, patch[1].ObjectPosition, uv.x);
	float4 v1 = lerp(patch[2].ObjectPosition, patch[3].ObjectPosition, uv.x);
	float3 objectPosition = lerp(v0, v1, uv.y);

	float2 uv1 = lerp(patch[0].TextureCoordinate, patch[1].TextureCoordinate, uv.x);
	float2 uv2 = lerp(patch[2].TextureCoordinate, patch[3].TextureCoordinate, uv.x);
	OUT.TextureCoordinate = lerp(uv1, uv2, uv.y);

	objectPosition.y = SubdivisionStrength * (ColorTexture.SampleLevel(ColorSampler, OUT.TextureCoordinate, 0).r - 0.5f);
	OUT.Position = mul(float4(objectPosition, 1.0f), ViewProjection);

	return OUT;
}

//[maxvertexcount(6)]
//void geometry_shader(point VS_OUTPUT IN[1], uint primitiveID : SV_PrimitiveID, inout TriangleStream<GS_OUTPUT> triangleStream)
//{
//	GS_OUTPUT OUT = (GS_OUTPUT)0;
//
//	float2 halfSize = IN[0].Size / 2.0f;
//	float3 direction = CameraPosition - IN[0].Position.xyz;
//	float3 right = cross(normalize(direction), CameraUp);
//
//	float3 offsetX = halfSize.x * right * (primitiveID+1);
//	float3 offsetY = halfSize.y * CameraUp * (primitiveID+1);
//
//	float4 vertices[4];
//	vertices[0] = float4(IN[0].Position.xyz + offsetX - offsetY, 1.0f);
//	vertices[1] = float4(IN[0].Position.xyz + offsetX + offsetY, 1.0f);
//	vertices[2] = float4(IN[0].Position.xyz - offsetX + offsetY, 1.0f);
//	vertices[3] = float4(IN[0].Position.xyz - offsetX - offsetY, 1.0f);
//
//	OUT.Position = mul(vertices[0], ViewProjection);
//	OUT.TextureCoordinate = QuadUV[0];
//	triangleStream.Append(OUT);
//
//	OUT.Position = mul(vertices[1], ViewProjection);
//	OUT.TextureCoordinate = QuadUV[1];
//	triangleStream.Append(OUT);
//
//	OUT.Position = mul(vertices[2], ViewProjection);
//	OUT.TextureCoordinate = QuadUV[2];
//	triangleStream.Append(OUT);
//
//	triangleStream.RestartStrip();
//
//	OUT.Position = mul(vertices[0], ViewProjection);
//	OUT.TextureCoordinate = QuadUV[0];
//	triangleStream.Append(OUT);
//
//	OUT.Position = mul(vertices[2], ViewProjection);
//	OUT.TextureCoordinate = QuadUV[2];
//	triangleStream.Append(OUT);
//
//	OUT.Position = mul(vertices[3], ViewProjection);
//	OUT.TextureCoordinate = QuadUV[3];
//	triangleStream.Append(OUT);
//}



//float4 pixel_shader_texture(GS_OUTPUT IN) : SV_Target
//{
//	return ColorTexture.Sample(ColorSampler, IN.TextureCoordinate);
//}

float4 pixel_shader_color(DS_OUTPUT IN) : SV_Target
{
	return float4(ColorTexture.Sample(ColorSampler, IN.TextureCoordinate).xxx, 1.0f);
}



technique11 main11
{
	/*pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, vertex_shader_texture()));
		SetGeometryShader(CompileShader(gs_5_0, geometry_shader()));
		SetPixelShader(CompileShader(ps_5_0, pixel_shader_texture()));
	}*/

	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
		SetHullShader(CompileShader(hs_5_0, hull_shader()));
		SetDomainShader(CompileShader(ds_5_0, domain_shader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, pixel_shader_color()));

		//SetRasterizerState(Wireframe);
	}
};
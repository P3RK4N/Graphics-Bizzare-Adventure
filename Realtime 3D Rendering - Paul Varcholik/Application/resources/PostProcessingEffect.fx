
static const float3 s_GrayScaleIntensity = { 0.299f, 0.587f, 0.114f };

Texture2D ColorTexture;
Texture2D DistortionTexture;

SamplerState TrilinearSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

SamplerState TrilinearBlurSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

struct VS_INPUT
{
	float4 ObjectPosition : POSITION;
	float2 TextureCoordinate : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 Position : SV_Position;
	float2 TextureCoordinate : TEXCOORD;
};

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;

	OUT.Position = IN.ObjectPosition;
	OUT.TextureCoordinate = IN.TextureCoordinate;

	return OUT;
}

float4 grayscale_pixel_shader(VS_OUTPUT IN) : SV_Target
{
	float4 color = ColorTexture.Sample(TrilinearSampler, IN.TextureCoordinate);
	float intensity = dot(color.rgb, s_GrayScaleIntensity);

	return intensity < 0.5f ? float4(0.0f, 0.0f, 0.0f, 1.0f) : color; // float4(intensity.rrr, color.a);
}

float4 inverse_pixel_shader(VS_OUTPUT IN) : SV_Target
{
	float4 color = ColorTexture.Sample(TrilinearSampler, IN.TextureCoordinate);
	return float4(1 - color.rgb, color.a);
}

float4 nothing_pixel_shader(VS_OUTPUT IN) : SV_Target
{
	return ColorTexture.Sample(TrilinearSampler, IN.TextureCoordinate);
}

float4 distortion_pixel_shader(VS_OUTPUT IN) : SV_Target
{
	float2 distortion = DistortionTexture.Sample(TrilinearSampler, IN.TextureCoordinate * 0.5f).xy - 0.5f;
	float2 offset = IN.TextureCoordinate + 0.02f * distortion * 15.5f;

	if(offset.x < 0.0f || offset.x > 1.0f || offset.y < 0.0f || offset.y > 1.0f)
		return float4(0.0f, 0.0f, 0.0f, 1.0f);

	float4 color = ColorTexture.Sample(TrilinearBlurSampler, offset);
	return color;
}

float4 blur_pixel_shader(VS_OUTPUT IN) : SV_Target
{
	float tmpW[9] = { 0.019, 0.33, 0.33, 0.28, 0.28, 0.21, 0.21, 0.14, 0.14 };
	float2 tmpO[] =
	{
		float2(0.0f, 0.0f),
		float2(0.0011,0.002),
		float2(-0.0011,-0.002),
		float2(0.0027,0.0048),
		float2(-0.0027,-0.0048),
		float2(0.0043, 0.0076),
		float2(-0.0043,-0.0076),
		float2(0.0058, 0.0104),
		float2(-0.0058,-0.0104)
	};
	float4 color = (float4)0;
	for(int i = 0; i < 9; i++)
	{
		color += 
		(	
			ColorTexture.Sample(TrilinearBlurSampler, IN.TextureCoordinate + tmpO[i].x) + 
			ColorTexture.Sample(TrilinearBlurSampler, IN.TextureCoordinate + tmpO[i].y)
		) * tmpW[i];
	}
	return float4(color.rgb / 2.0f, 1.0f);
}

technique11 filter
{
	pass grayscale
	{
		SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
		SetPixelShader(CompileShader(ps_5_0, grayscale_pixel_shader()));
		SetGeometryShader(NULL);
	}

	pass nothing
	{
		SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
		SetPixelShader(CompileShader(ps_5_0, nothing_pixel_shader()));
		SetGeometryShader(NULL);
	}

	pass inverse
	{
		SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
		SetPixelShader(CompileShader(ps_5_0, inverse_pixel_shader()));
		SetGeometryShader(NULL);
	}

	pass blur
	{
		SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
		SetPixelShader(CompileShader(ps_5_0, blur_pixel_shader()));
		SetGeometryShader(NULL);
	}

	pass distortion
	{
		SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
		SetPixelShader(CompileShader(ps_5_0, distortion_pixel_shader()));
		SetGeometryShader(NULL);
	}
};


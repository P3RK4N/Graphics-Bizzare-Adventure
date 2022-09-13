static const float4 ColorWhite = { 1, 1, 1, 1 };

cbuffer CBufferPerFrame
{
	float4 AmbientColor = { 1.0f, 1.0f, 1.0f, 0.0f };
	float4 LightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	float3 LightPosition = { 0.0f, 0.0f, 0.0f };
	float LightRadius = 10.0f;
	float3 LightDirection = { 1.0f, 0.0f, 0.0f };
	float3 CameraPosition;
};

cbuffer CBufferPerObject
{
	float4x4 WorldViewProjection : WORLDVIEWPROJECTION;
	float4x4 WorldLightViewProjectionScale;
	float4x4 WorldLightViewProjection;
	float4x4 World : WORLD;
	float4	SpecularColor : SPECULAR = { 1.0f, 1.0f, 1.0f, 1.0f };
	float SpecularPower : SPECULARPOWER = 25.0f;
}

Texture2D ColorTexture;
Texture2D ProjectedTexture;

SamplerState ProjectedTextureSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = BORDER;
	AddressV = BORDER;
	BorderColor = ColorWhite;
};

SamplerState ColorSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

RasterizerState BackFaceCulling
{
	CullMode = BACK;
};

RasterizerState DepthBias
{
	DepthBias = 84000;
};

SamplerComparisonState PCFShadowMapSampler
{
	Filter = COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	AddressU = BORDER;
	AddressV = BORDER;
	BorderColor = ColorWhite;
	ComparisonFunc = LESS_EQUAL;
};

struct VS_INPUT
{
	float4 ObjectPosition : POSITION;
	float2 TextureCoordinate : TEXCOORD;
	float3 Normal : NORMAL;
};

struct VS_OUTPUT
{
	 float4 Position : SV_Position;
	 float3 Normal : NORMAL;
	 float2 TextureCoordinate : TEXCOORD0;
	 float3 WorldPosition : TEXCOORD1;
	 float Attenuation : TEXCOORD2;
	 float4 ProjectedTextureCoordinate : TEXCOORD3;
};

float4 SM_VS(VS_INPUT IN) : SV_Position
{
	return mul(IN.ObjectPosition, WorldLightViewProjection);
};

VS_OUTPUT PTM_VS(VS_INPUT IN)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;

	OUT.Position = mul(IN.ObjectPosition, WorldViewProjection);
	OUT.WorldPosition = mul(IN.ObjectPosition, World);
	OUT.TextureCoordinate = IN.TextureCoordinate;
	OUT.Normal = normalize(mul(float4(IN.Normal,0), World).xyz);
	float3 lightDirection = LightPosition - OUT.WorldPosition;
	OUT.Attenuation = saturate(1.0f - (length(lightDirection) / LightRadius));
	OUT.ProjectedTextureCoordinate = mul(IN.ObjectPosition, WorldLightViewProjectionScale);
	return OUT;
}

float4 PTM_PS(VS_OUTPUT IN) : SV_Target
{
	float4 OUT = (float4)0;

	float3 lightDirection = LightPosition - IN.WorldPosition;
	lightDirection = normalize(lightDirection);
	float3 viewDirection = normalize(CameraPosition-IN.WorldPosition);
	float3 normal = normalize(IN.Normal);
	float n_dot_l = dot(normal, lightDirection);
	float3 halfVector = normalize(lightDirection + viewDirection);
	float n_dot_h = dot(normal, halfVector);
	float4 color = ColorTexture.Sample(ColorSampler, IN.TextureCoordinate);
	float4 lightCoefficients = lit(n_dot_l,n_dot_h, SpecularPower);
	float3 ambient = AmbientColor.rgb * color.rgb * AmbientColor.a;
	float3 diffuse = LightColor.rgb * LightColor.a * lightCoefficients.y * color.rgb * IN.Attenuation;
	float3 specular = SpecularColor * min(lightCoefficients.z, color.w) * IN.Attenuation;

	IN.ProjectedTextureCoordinate.xyz /= IN.ProjectedTextureCoordinate.w;
	float pixelDepth = IN.ProjectedTextureCoordinate.z - 0.005f;
	float shadow = ProjectedTexture.SampleCmpLevelZero(PCFShadowMapSampler, IN.ProjectedTextureCoordinate.xy, pixelDepth).x;
	diffuse *= shadow;
	specular *= shadow;

	OUT.a = 1.0f;
	OUT.rgb = ambient + diffuse + specular;
	return OUT;
}

technique11 project_texture
{
	 pass p0
	 {
		SetVertexShader(CompileShader(vs_5_0, PTM_VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0,PTM_PS()));
		SetRasterizerState(BackFaceCulling);
	 }

	 pass shadow
	 {
		SetVertexShader(CompileShader(vs_5_0, SM_VS()));
		SetGeometryShader(NULL);
		SetPixelShader(NULL);
		SetRasterizerState(BackFaceCulling);
	 }
}

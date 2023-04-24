cbuffer cbuff0:register(b0)
{
	float4 color;//F
	matrix world;//3D•ÏŠ·s—ñ
	matrix viewproj;//3D•ÏŠ·s—ñ
};

struct VSOutput
{
	float4 svpos:SV_POSITION;
};
cbuffer cbuff0:register(b0)
{
	matrix viewproj; // ビュープロジェクション行列
	matrix world; // ワールド行列
	float3 cameraPos; // カメラ座標（ワールド座標）
};

struct VSOutput
{
	float4 svpos:SV_POSITION;
	float3 uv : TEXCOORD;
};
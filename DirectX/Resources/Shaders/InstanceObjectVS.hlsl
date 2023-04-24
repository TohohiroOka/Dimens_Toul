#include "InstanceObject.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, uint instNo : SV_InstanceID)
{
	// 法線にワールド行列によるスケーリング・回転を適用
	float4 wnormal = normalize(mul(matWorld[instNo], float4(normal, 0)));

	VSOutput output; // ピクセルシェーダーに渡す値
	output.svpos = mul(mul(viewproj, matWorld[instNo]), pos);
	output.worldpos = mul(matWorld[instNo], pos);
	output.normal = wnormal.xyz;
	output.uv = uv;
	output.instNo = instNo;

	return output;
}
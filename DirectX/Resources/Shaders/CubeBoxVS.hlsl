#include "CubeBox.hlsli"

VSOutput main(float4 pos:POSITION, float3 uv : TEXCOORD)
{
	// ピクセルシェーダーに渡す値
	VSOutput output;
	output.svpos = mul(mul(viewproj, world), pos);
	output.uv = uv;

	return output;
}
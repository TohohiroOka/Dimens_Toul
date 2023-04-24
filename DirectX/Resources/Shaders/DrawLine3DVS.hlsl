#include "DrawLine3D.hlsli"

VSOutput main( float4 pos : POSITION ) : SV_POSITION
{
	//ピクセルシェーダーに渡す値
	VSOutput output;
	output.svpos = mul(mul(viewproj, world), pos);

	return output;
}
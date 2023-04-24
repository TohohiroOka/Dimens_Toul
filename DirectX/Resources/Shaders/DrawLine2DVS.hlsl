#include "DrawLine2D.hlsli"

VSOutput main(float4 pos:POSITION)
{
	VSOutput output;//ピクセルシェーダーに渡す値
	output.svpos = mul(mat, pos);//座標に行列を乗算
	return output;
}

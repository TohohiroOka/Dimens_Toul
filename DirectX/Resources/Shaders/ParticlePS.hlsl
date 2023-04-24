#include "Particle.hlsli"

Texture2D<float> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

PSOutput main(GSOutput input)
{
	float4 color = tex.Sample(smp, input.uv);
	color = color * input.color;

	//ブルーム処理
	float4 bloom = float4(0, 0, 0, 0);
	if (isBloom)
	{
		bloom = input.color;
	}

	PSOutput output;
	output.target0 = float4(color.rgb, input.color.w);
	output.target1 = bloom;

	return output;
}
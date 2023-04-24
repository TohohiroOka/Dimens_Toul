cbuffer cbuff0 : register(b0)
{
	matrix mat; // ３Ｄ変換行列
	matrix matBillboard;//ビルボード行列
	uint isBloom;//ブルームの有無
};

// 頂点シェーダーからジオメトリシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 pos : POSITION; // システム用頂点座標
	float scale : SCALE;//スケール
	float4 color:COLOR;//色
};

//ジオメトリシェーダーからピクセルシェーダーへの出力
struct GSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float2 uv  :TEXCOORD; // uv値
	float4 color:COLOR;//色
};

//ピクセルシェーダーから実行処理へのやり取りに使用する構造体
struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
};
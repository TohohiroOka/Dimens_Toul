cbuffer cbuff0 : register(b0)
{
	float4 baseColor;
	matrix viewproj; // ビュープロジェクション行列
	matrix world; // ワールド行列
	float3 cameraPos; // カメラ座標（ワールド座標）
	uint isSkinning;//スキニング
	uint isBloom;//ブルームの有無
	uint isToon;//トゥーンの有無
	uint isOutline;//アウトラインの有無
	uint isLight;
};

cbuffer cbuff1 : register(b1)
{
	float3 m_baseColor;//ベースカラー
	float3 m_ambient; //アンビエント係数
	float3 m_diffuse; //ディフューズ係数
	float m_metalness; //金属度
	float m_specular; //スペキュラー係数
	float m_roughness; //粗さ
	float m_alpha; //アルファ
};

// 平行光源の数
static const int DIRLIGHT_NUM = 3;

struct DirLight
{
	float3 lightv; // ライトへの方向の単位ベクトル
	float3 lightcolor; // ライトの色(RGB)
	uint active;
};

// 点光源の数
static const int POINTLIGHT_NUM = 3;

struct PointLight
{
	float3 lightpos;    // ライト座標
	float3 lightcolor;  // ライトの色(RGB)
	float3 lightatten;	// ライト距離減衰係数
	uint active;
};

// スポットライトの数
static const int SPOTLIGHT_NUM = 3;

struct SpotLight
{
	float3 lightv;		// ライトの光線方向の逆ベクトル（単位ベクトル）
	float3 lightpos;    // ライト座標
	float3 lightcolor;  // ライトの色(RGB)
	float3 lightatten;	// ライト距離減衰係数
	float2 lightfactoranglecos; // ライト減衰角度のコサイン
	uint active;
};

// 丸影の数
static const int CIRCLESHADOW_NUM = 3;

struct CircleShadow
{
	float3 dir;		// 投影方向の逆ベクトル（単位ベクトル）
	float3 casterPos;    // キャスター座標
	float  distanceCasterLight;	// キャスターとライトの距離
	float3 atten;	// 距離減衰係数
	float2 factorAngleCos; // 減衰角度のコサイン
	uint active;
};

cbuffer cbuff2 : register(b2)
{
	float3 ambientColor;
	DirLight dirLights[DIRLIGHT_NUM];
	PointLight pointLights[POINTLIGHT_NUM];
	SpotLight spotLights[SPOTLIGHT_NUM];
	CircleShadow circleShadows[CIRCLESHADOW_NUM];
}

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float4 worldpos : POSITION0; // ワールド座標
	float3 normal :NORMAL; // 法線
	float2 uv  :TEXCOORD; // uv値
};

//ピクセルシェーダーから実行処理へのやり取りに使用する構造体
struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
	float4 target2 : SV_TARGET2;
};
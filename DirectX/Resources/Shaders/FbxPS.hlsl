#include "Fbx.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
TextureCube cubeTex:register(t1);//1番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

//πの定義
static const float PI = 3.141592654f;
//反射点の法線ベクトル
static float3 N;

//関数定義
float4 SetBloom(float4 shadecolor, float4 texcolor, float4 color);
float4 SetToon(float4 shadecolor);
float3 BRDF(float3 L, float3 V, float3 color);
float SchlickFresnel(float f0, float f90, float cosine);
float3 CookTorranceSpecular(float NdotL, float NdotV, float NdotH, float LdotH);
float DistributionGGX(float alpha, float NdotH);
float3 SchlickFresnel3(float3 f0, float3 f90, float cosine);
float3 DisneyFresnel(float LdotH);
float GeometricSmith(float cosine);

float4 main(VSOutput input) : SV_TARGET
{
	// テクスチャマッピング
	float4 texcolor = tex.Sample(smp, input.uv);

	//キューブマップ
	float4 cubeColor = cubeTex.Sample(smp, input.normal);

	// 光沢度
	const float shininess = 4.0f;

	// 頂点から視点への方向ベクトル
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	// 環境反射光
	float3 ambient = m_ambient;

	// シェーディングによる色
	float4 shadecolor = float4(ambientColor * ambient, m_alpha);

	N = input.normal;
	//出力色
	float3 finalRGB = float3(0, 0, 0);


	//平行光源
	for (int i = 0; i < DIRLIGHT_NUM; i++)
	{
		if (dirLights[i].active)
		{
			//// ライトに向かうベクトルと法線の内積
			//float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);

			//// 反射光ベクトル
			//float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
			//// 拡散反射光
			//float3 diffuse = dotlightnormal * m_diffuse;
			//// 鏡面反射光
			//float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			//// 全て加算する
			//shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor;

			finalRGB += BRDF(dirLights[i].lightv, eyedir, cubeColor.rgb) * dirLights[i].lightcolor;
		}
	}

	//点光源
	for (i = 0; i < POINTLIGHT_NUM; i++)
	{
		if (pointLights[i].active)
		{
			//ライトへのベクトル
			float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
			//ベクトルの長さ
			float d = length(lightv);
			//正規化し、単位ベクトルにする
			lightv = normalize(lightv);
			//距離減衰係数
			float atten = 1.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d + pointLights[i].lightatten.z * d * d);
			//ライトに向かうベクトルと法線の内積
			float3 dotlightnormal = dot(lightv, input.normal);
			//反射光ベクトル
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			// 拡散反射光
			float3 diffuse = dotlightnormal * m_diffuse;
			// 鏡面反射光
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			// 全て加算する
			shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor;
		}
	}

	// スポットライト
	for (i = 0; i < SPOTLIGHT_NUM; i++)
	{
		if (spotLights[i].active)
		{
			// ライトへの方向ベクトル
			float3 lightv = spotLights[i].lightpos - input.worldpos.xyz;
			//ベクトルの長さ
			float d = length(lightv);
			//正規化し、単位
			lightv = normalize(lightv);
			// 距離減衰係数
			float atten = saturate(1.0f / (spotLights[i].lightatten.x + spotLights[i].lightatten.y * d + spotLights[i].lightatten.z * d * d));
			// 角度減衰
			float cos = dot(lightv, spotLights[i].lightv);
			// 減衰開始角度から、減衰終了角度にかけて減衰
			// 減衰開始角度の内側は1倍 減衰終了角度の外側は0倍の輝度
			float angleatten = smoothstep(spotLights[i].lightfactoranglecos.y, spotLights[i].lightfactoranglecos.x, cos);
			// 角度減衰を乗算
			atten *= angleatten;
			// ライトに向かうベクトルと法線の内積
			float3 dotlightnormal = dot(lightv, input.normal);
			// 反射光ベクトル
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			// 拡散反射光
			float3 diffuse = dotlightnormal * m_diffuse;
			// 鏡面反射光
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			// 全て加算する
			shadecolor.rgb += atten * (diffuse + specular) * spotLights[i].lightcolor;
		}
	}

	// 丸影
	for (i = 0; i < CIRCLESHADOW_NUM; i++)
	{
		if (circleShadows[i].active)
		{
			// オブジェクト表面からキャスターへのベクトル
			float3 casterv = circleShadows[i].casterPos - input.worldpos.xyz;
			// 光線方向での距離
			float d = dot(casterv, circleShadows[i].dir);
			// 距離減衰係数
			float atten = saturate(1.0f / (circleShadows[i].atten.x + circleShadows[i].atten.y * d + circleShadows[i].atten.z * d * d));
			// 距離がマイナスなら0にする
			atten *= step(0, d);
			// ライトの座標
			float3 lightpos = circleShadows[i].casterPos + circleShadows[i].dir * circleShadows[i].distanceCasterLight;
			//  オブジェクト表面からライトへのベクトル（単位ベクトル）
			float3 lightv = normalize(lightpos - input.worldpos.xyz);
			// 角度減衰
			float cos = dot(lightv, circleShadows[i].dir);
			// 減衰開始角度から、減衰終了角度にかけて減衰
			// 減衰開始角度の内側は1倍 減衰終了角度の外側は0倍の輝度
			float angleatten = smoothstep(circleShadows[i].factorAngleCos.y, circleShadows[i].factorAngleCos.x, cos);
			// 角度減衰を乗算
			atten *= angleatten;

			// 全て減算する
			shadecolor.rgb -= atten;
		}
	}

	//ポストエフェクト処理

	//ブルーム処理
	float4 bloom = float4(0, 0, 0, 0);
	if (isBloom)
	{
		bloom = SetBloom(shadecolor, texcolor, color);
	}

	//トゥーンシェード
	if (isToon)
	{
		shadecolor = SetToon(shadecolor);
	}

	// シェーディングによる色で描画
	float4 output = float4(finalRGB.rgb, color.w);
	return output;
}

/// <summary>
/// ブルームのセット
/// </summary>
float4 SetBloom(float4 shadecolor, float4 texcolor, float4 color)
{
	//光度値の抽出
	float LuminousIntensity = dot(shadecolor.rgb * texcolor.rgb, float3(0.2125, 0.7154, 0.0712));

	//ブルームをかける場所
	float4 bloomColor = step(1.0, LuminousIntensity) * texcolor * color;

	return bloomColor;
}

/// <summary>
/// トゥーンのセット
/// </summary>
float4 SetToon(float4 shadecolor)
{
	//トゥーンの色範囲
	float toonLighrRange = 0.525;
	//明暗の中間幅
	float contourWidth = 0.1;

	//明るい
	float3 bright;
	bright.r = smoothstep(toonLighrRange, toonLighrRange + contourWidth, shadecolor.r);
	bright.g = smoothstep(toonLighrRange, toonLighrRange + contourWidth, shadecolor.g);
	bright.b = smoothstep(toonLighrRange, toonLighrRange + contourWidth, shadecolor.b);
	//暗い
	float3 dark;
	dark.r = (1 - smoothstep(toonLighrRange, toonLighrRange + contourWidth, shadecolor.r));
	dark.g = (1 - smoothstep(toonLighrRange, toonLighrRange + contourWidth, shadecolor.g));
	dark.b = (1 - smoothstep(toonLighrRange, toonLighrRange + contourWidth, shadecolor.b));

	//現在の色
	return float4(bright + dark, 1);
}

/// <summary>
/// 双方向反射分布関数
/// </summary>
float3 BRDF(float3 L, float3 V,float3 color)
{
	//法線とライト方向の内積
	float NdotL = dot(N, L);
	//法線とカメラ方向の内積
	float NdotV = dot(N, V);
	//どちらかが90°以上なら黒を返す
	if (NdotL < 0 || NdotV < 0) { return float3(0, 0, 0); }

	//ライト方向とカメラ方向の中間
	float3 H = normalize(L + V);
	//法線とハーフベクトルの内積
	float NdotH = dot(N, H);
	//ライトとハーフベクトルの内積
	float LdotH = dot(L, H);

	//拡散反射率
	float diffuseReflectance = 1.0f / PI;

	//90度の場合のラフネス減衰処理
	float energyBias = 0.5f * m_roughness;
	//入射角が90度の場合の拡散反射率
	float Fd90 = energyBias + 2.0f * LdotH * LdotH * m_roughness;
	//入っていく時の拡散反射率
	float FL = SchlickFresnel(1.0f, Fd90, NdotL);
	//出ていく時の拡散反射率
	float FV = SchlickFresnel(1.0f, Fd90, NdotV);
	//最終的な反射率にラフネスによる減衰を追加
	float energyFactor = lerp(1.0f, 1.0f / 1.51f, m_roughness);
	//入って出ていくまでの拡散反射率
	float Fd = FL * FV * energyFactor;

	//拡散反射光
	float3 diffuseColor = diffuseReflectance * Fd * m_baseColor * (1 - m_metalness);

	//鏡面反射項
	float3 specularColor = CookTorranceSpecular(NdotL, NdotV, NdotH, LdotH);

	return diffuseColor + specularColor * color;
}

/// <summary>
/// フルネルの近似値(float)
/// </summary>
float SchlickFresnel(float f0, float f90, float cosine)
{
	float m = saturate(1 - cosine);
	float m2 = m * m;
	float m5 = m2 * m2 * m;
	return lerp(f0, f90, m5);
}

/// <summary>
/// 鏡面反射の計算
/// </summary>
float3 CookTorranceSpecular(float NdotL, float NdotV, float NdotH, float LdotH)
{
	//D項
	float Ds = DistributionGGX(m_roughness * m_roughness, NdotH);
	//F項(分布 : Distribution)
	float3 Fs = DisneyFresnel(LdotH);
	//G項(幾何減数 : Geometry attenuation)
	float Gs = GeometricSmith(NdotL) * GeometricSmith(NdotV);
	//m項(分母)
	float m = 1.0f * NdotL * NdotV;

	//合成して鏡面反射の色を返す
	return Ds * Fs * Gs / m;
}

/// <summary>
/// D項
/// </summary
float DistributionGGX(float alpha, float NdotH)
{
	float alpha2 = alpha * alpha;
	float t = NdotH * NdotH * (alpha2 - 1.0f) + 1.0f;
	return alpha2 / (PI * t * t);
}

/// <summary>
/// フルネルの近似値(float3)
/// </summary
float3 SchlickFresnel3(float3 f0, float3 f90, float cosine)
{
	float m = saturate(1 - cosine);
	float m2 = m * m;
	float m5 = m2 * m2 * m;
	return lerp(f0, f90, m5);
}

/// <summary>
/// ディズニーのフルネル計算
/// </summary
float3 DisneyFresnel(float LdotH)
{
	//F項(フルネル : Fresnel)
	//輝度
	float liminance = 0.3f * m_baseColor.r + 0.6f * m_baseColor.g + 0.1f * m_baseColor.b;
	//色合い
	float3 tintColor = m_baseColor / liminance;
	//非金属の鏡面反射色の計算
	float3 nonMetarlColor = m_specular * 0.08f * tintColor;
	//metalnessによる色補間 金属の場合はベースカラー
	float3 specularColor = lerp(nonMetarlColor, m_baseColor, m_metalness);

	//NdptHの割合でSchlickFresnel補間
	return SchlickFresnel3(specularColor, float3(1, 1, 1), LdotH);
}

/// <summary>
/// UE4のSmithモデル
/// </summary
float GeometricSmith(float cosine)
{
	float k = (m_roughness + 1.0f);
	k = k * k / 8.0f;

	return cosine / (cosine * (1.0f - k) + k);
}
#pragma once

/// <summary>
/// 衝突判定用関連の型定義
/// </summary>

enum COLILSION_SHAPE_TYPE
{
	SHAPE_UNKNOWN = -1, // 未設定

	COLLISIONSHAPE_SPHERE, // 球
	COLLISIONSHAPE_MESH, // メッシュ
};

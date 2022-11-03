#pragma once

#include "WorldTransform.h"
#include "PrimitiveDrawer.h"

namespace Affine
{
	//度からラジアンに変換する定数
	constexpr float Deg2Rad = 3.1415926535897931f / 180.0f;
	constexpr float PIHalf = 3.1415926535897931f / 2.0f;

	/// <summary>
	/// 単位行列
	/// </summary>
	/// <param name="matrix"></param>
	void CreateMatIdentity(Matrix4& matrix);

	/// <summary>
	/// スケール
	/// </summary>
	/// <param name="matrix"></param>
	/// <param name="scale"></param>
	void CreateMatScale(Matrix4& matrix, Vector3 scale);

	/// <summary>
	/// 回転
	/// </summary>
	/// <param name="matrix"></param>
	/// <param name="rot"></param>
	void CreateMatRot(Matrix4& matrix, Vector3 rot);

	/// <summary>
	/// 平行移動
	/// </summary>
	/// <param name="matrix"></param>
	/// <param name="trans"></param>
	void CreateMatTrans(Matrix4& matrix, Vector3 trans);

	void CreateAffine(WorldTransform& worldTransform);

	/// <summary>
	/// ベクトルと行列の掛け算
	/// </summary>
	/// <param name="matrix"></param>
	/// <param name="vector"></param>
	/// <returns></returns>
	Vector3 MatVector(Matrix4 matrix, Vector3 vector);

	Vector3 GetWorldTransform(Matrix4 matrix);
}

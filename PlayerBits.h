#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Input.h"
#include "MathMatrix.h"

class PlayerBits
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initilize(const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(WorldTransform worldTransform, Vector3 V);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(ViewProjection* viewProjection);

	bool IsDead() const { return isDead_; }

private:
	//ワールド変換データ
	WorldTransform worldTransformBit[6];

	int bitMax = 6;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// モデル
	std::unique_ptr<Model> model_;

	const float PI = 3.14159f;

	float rotaSpeed = 0;

	//デスフラグ
	bool isDead_ = false;

};

#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Affine.h"
#include "Input.h"
#include "PlayerBullet.h"

class PlayerModel
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection* viewProjection);

	void Attack();

	//ワールド行列のゲッター
	WorldTransform GetWorldTransform() { return worldTransform_; }

	const std::list < std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

private:
	const float PI = 3.14159f;

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	std::unique_ptr<Model> model_;
	
	Input* input_ = nullptr;

	//ラジアン
	float radian;

	Vector3 playerPos;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
};
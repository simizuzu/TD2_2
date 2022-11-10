#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Affine.h"
#include "Input.h"

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

	//ワールド行列のゲッター
	WorldTransform GetWorldTransform() { return worldTransform_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	std::unique_ptr<Model> model_;
	
	Input* input_ = nullptr;

	float radian = 0.0f;
};
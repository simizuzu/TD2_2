#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Affine.h"

class Skydome
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection* viewProjection);
private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	//uint32_t textureHandle_ = 0u;
};


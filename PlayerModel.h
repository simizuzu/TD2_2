#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Affine.h"

class PlayerModel
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection* viewProjection);
private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	std::unique_ptr<Model> model_;
};
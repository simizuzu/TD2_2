#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Affine.h"

class Enemy
{
public:
	//初期化
	void Initialize(Model* model);

	//更新
	void Update();

	//描画
	void Draw(ViewProjection* viewProjection);

	//シェイク
	void Shake();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;

	//シェイク時の移動距離
	float shakeX = 0.2f;
};


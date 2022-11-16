#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Affine.h"

//シェイク開始する時間
static const float SHAKE_START = 50.0f;

class Enemy
{
public:
	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw(ViewProjection* viewProjection);

	//登場時の動き
	void AppearMove();

	//シェイク
	void Shake();

	//ワールド行列のゲッター
	WorldTransform GetWorldTransform() { return worldTransform_; }
	//登場時の経過時間のゲッター
	float GetAppearTimer() { return appearTimer; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	std::unique_ptr<Model> model_;

	//シェイク時の移動距離
	float shakeVal;
	//登場時の経過時間
	float appearTimer;


};


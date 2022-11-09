#include "Enemy.h"
#include <cassert>

void Enemy::Initialize(Model* model)
{
	// NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	//ワールド変換データの初期化
	worldTransform_.Initialize();

	//初期座標
	worldTransform_.translation_ = { 0.0f,20.0f,0.0f };
	worldTransform_.scale_ = { 5.0f,5.0f,5.0f };

	//アフィン変換
	Affine::CreateAffine(worldTransform_);

	//行列更新
	worldTransform_.TransferMatrix();
}

void Enemy::Update()
{
	if (worldTransform_.matWorld_.m[3][1] > 0.0f)
	{
		Affine::CreateMatTrans(worldTransform_.matWorld_, { 0.0f,-0.1f,0.0f });
	}
	else
	{
		Shake();
	}
	//行列更新
	worldTransform_.TransferMatrix();
}

void Enemy::Draw(ViewProjection* viewProjection)
{
	model_->Draw(worldTransform_, *viewProjection);
}

void Enemy::Shake()
{
	worldTransform_.translation_ = { shakeX,0.0f,0.0f };
	Affine::CreateAffine(worldTransform_);

	shakeX = -shakeX;

}
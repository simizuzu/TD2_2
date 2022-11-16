#include "Enemy.h"

void Enemy::Initialize()
{
	//シェイク時の移動距離
	shakeVal = 1.000f;
	//登場時の経過時間
	appearTimer = 0.0f;

	//ワールド変換データの初期化
	worldTransform_.Initialize();

	//初期座標
	worldTransform_.translation_ = { 0.0f,30.0f,0.0f };
	worldTransform_.scale_ = { 10.0f,10.0f,10.0f };

	//アフィン変換
	Affine::CreateAffine(worldTransform_);

	//行列更新
	worldTransform_.TransferMatrix();

	model_.reset(Model::CreateFromOBJ("Enemy", true));
}

void Enemy::Update()
{
	worldTransform_.translation_ = { 0.0f,0.0f,0.0f };

	//アフィン変換
	Affine::CreateAffine(worldTransform_);
	//行列更新
	worldTransform_.TransferMatrix();
}

void Enemy::Draw(ViewProjection* viewProjection)
{
	model_->Draw(worldTransform_, *viewProjection);
}

void Enemy::AppearMove()
{
	//Y0までゆっくり下降
	if (worldTransform_.matWorld_.m[3][1] > 0.0f) {
		Affine::CreateMatTrans(worldTransform_.matWorld_, { 0.0f,-0.1f,0.0f });
	}
	else {
		//到着したらタイマーを増やす
		appearTimer++;
	}

	//一定時間たったらシェイク
	if (appearTimer >= SHAKE_START) {

		//一定時間たったらシェイク量を少しづつ減らす
		if (shakeVal > 0 && appearTimer >= 150.0f) {
			shakeVal -= 0.01f;
		}

		Shake();
	}

	//行列更新
	worldTransform_.TransferMatrix();
}

void Enemy::Shake()
{
	worldTransform_.translation_ = { 0.0f,shakeVal,0.0f };
	Affine::CreateAffine(worldTransform_);

	shakeVal = -shakeVal;

}
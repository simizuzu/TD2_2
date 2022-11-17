#include "EnemyBullet.h"
#include "Affine.h"

void EnemyBullet::Initialize(const Vector3& position, const Vector3& velocity) {

	//ワールド変換データの初期化
	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = { position.x, position.y, position.z };

	//大きさ設定
	worldTransform_.scale_ = { 3.0f,3.0f,3.0f };

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	//アフィン変換
	Affine::CreateAffine(worldTransform_);

	//行列更新
	worldTransform_.TransferMatrix();

	//モデル初期化
	model_.reset(Model::CreateFromOBJ("Enemy", true));

}

void EnemyBullet::Update() {
	//座標を移動させる
	worldTransform_.translation_ += velocity_;

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	//アフィン変換
	Affine::CreateAffine(worldTransform_);
	//行列更新
	worldTransform_.TransferMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}
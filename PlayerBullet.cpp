#include "PlayerBullet.h"
#include <cassert>

void PlayerBullet::Initilize(const Vector3& position, const Vector3& velocity) {

	//テクスチャ読み込み
	model_ = Model::CreateFromOBJ("Player", true);

	//ワールドトランスフォームの初期化
	worldTransformBullet_.rotation_ = { 0.0f, 0.0f, 0.0f };

	worldTransformBullet_.translation_ = position;

	worldTransformBullet_.scale_ = { 0.1, 0.1, 0.1 };

	worldTransformBullet_.Initialize();

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}

void PlayerBullet::Update(WorldTransform worldTransform,float hp) {

	//座標を移動させる(1フレーム分の移動量を足す)
	worldTransformBullet_.translation_ += velocity_;

	//時間経過でデス
	if (--deathTimer_ <= 0 || --hp <= 0) {
		isDead_ = true;
	}

	worldTransformBullet_.matWorld_ = MathUtility::Matrix4Identity();

	worldTransformBullet_.matWorld_ =
		ScaleMatrix4(worldTransformBullet_.matWorld_, worldTransformBullet_.scale_);

	worldTransformBullet_.matWorld_ =
		RotationZMatrix4(worldTransformBullet_.matWorld_, worldTransformBullet_.rotation_);

	worldTransformBullet_.matWorld_ =
		RotationYMatrix4(worldTransformBullet_.matWorld_, worldTransformBullet_.rotation_);

	worldTransformBullet_.matWorld_ =
		MoveMatrix4(worldTransformBullet_.matWorld_, worldTransformBullet_.translation_);

	//行列の再計算
	worldTransformBullet_.TransferMatrix();

}

void PlayerBullet::Draw(ViewProjection* viewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransformBullet_, *viewProjection);
}

Vector3 PlayerBullet::GetWorldPos() {
	worldTransformBulletPos_.x = worldTransformBullet_.matWorld_.m[3][0];
	worldTransformBulletPos_.y = worldTransformBullet_.matWorld_.m[3][1];
	worldTransformBulletPos_.z = worldTransformBullet_.matWorld_.m[3][2];

	return worldTransformBulletPos_;
}

void PlayerBullet::OnCollision() {
	isDead_ = true;
};

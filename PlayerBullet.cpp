#include "PlayerBullet.h"
#include <cassert>

//行列計算用関数
Matrix4 ScaleMatrix4(Matrix4 matWorld, Vector3 scale);

Matrix4 RotationXMatrix4(Matrix4 matWorld, Vector3 rotation);

Matrix4 RotationYMatrix4(Matrix4 matWorld, Vector3 rotation);

Matrix4 RotationZMatrix4(Matrix4 matWorld, Vector3 rotation);

Matrix4 MoveMatrix4(Matrix4 matWorld, Vector3 translation);

void PlayerBullet::Initilize(const Vector3& position, const Vector3& velocity) {
	//model_ = Model::Create();

	//テクスチャ読み込み
	model_.reset(Model::CreateFromOBJ("Player", true));

	//ワールドトランスフォームの初期化
	worldTransformBullet_.rotation_ = { 0.0f, 0.0f, 0.0f };

	worldTransformBullet_.translation_ = position;

	worldTransformBullet_.scale_ = { 0.1, 0.1, 0.1 };

	worldTransformBullet_.Initialize();

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}

void PlayerBullet::Update(WorldTransform worldTransform) {

	//座標を移動させる(1フレーム分の移動量を足す)
	worldTransformBullet_.translation_ += velocity_;

	//時間経過でデス
	if (--deathTimer_ <= 0) {
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

//行列計算用関数

Matrix4 ScaleMatrix4(Matrix4 matWorld, Vector3 scale)
{
	Matrix4 matScale = MathUtility::Matrix4Identity();

	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;

	return matWorld *= matScale;
}

Matrix4 RotationXMatrix4(Matrix4 matWorld, Vector3 rotation)
{
	Matrix4 matRotX = MathUtility::Matrix4Identity();

	matRotX.m[1][1] = cosf(rotation.x);
	matRotX.m[1][2] = sinf(rotation.x);
	matRotX.m[2][1] = -sinf(rotation.x);
	matRotX.m[2][2] = cosf(rotation.x);

	return matWorld *= matRotX;
}

Matrix4 RotationYMatrix4(Matrix4 matWorld, Vector3 rotation)
{
	Matrix4 matRotY = MathUtility::Matrix4Identity();

	matRotY.m[0][0] = cosf(rotation.y);
	matRotY.m[0][2] = -sinf(rotation.y);
	matRotY.m[2][0] = sinf(rotation.y);
	matRotY.m[2][2] = cosf(rotation.y);

	return matWorld *= matRotY;
}

Matrix4 RotationZMatrix4(Matrix4 matWorld, Vector3 rotation)
{
	Matrix4 matRotZ = MathUtility::Matrix4Identity();

	matRotZ.m[0][0] = cosf(rotation.z);
	matRotZ.m[0][1] = sinf(rotation.z);
	matRotZ.m[1][0] = -sinf(rotation.z);
	matRotZ.m[1][1] = cosf(rotation.z);

	return matWorld *= matRotZ;
}

Matrix4 MoveMatrix4(Matrix4 matWorld, Vector3 translation)
{
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = translation.x;
	matTrans.m[3][1] = translation.y;
	matTrans.m[3][2] = translation.z;

	return matWorld *= matTrans;
}
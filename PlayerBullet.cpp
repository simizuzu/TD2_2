#include "PlayerBullet.h"
#include <cassert>

void PlayerBullet::Initilize(const Vector3& position, const Vector3& velocity) {

	//�e�N�X�`���ǂݍ���
	model_ = Model::CreateFromOBJ("Player", true);

	//���[���h�g�����X�t�H�[���̏�����
	worldTransformBullet_.rotation_ = { 0.0f, 0.0f, 0.0f };

	worldTransformBullet_.translation_ = position;

	worldTransformBullet_.scale_ = { 0.1, 0.1, 0.1 };

	worldTransformBullet_.Initialize();

	//�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;
}

void PlayerBullet::Update(WorldTransform worldTransform,float hp) {

	//���W���ړ�������(1�t���[�����̈ړ��ʂ𑫂�)
	worldTransformBullet_.translation_ += velocity_;

	//���Ԍo�߂Ńf�X
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

	//�s��̍Čv�Z
	worldTransformBullet_.TransferMatrix();

}

void PlayerBullet::Draw(ViewProjection* viewProjection) {
	// 3D���f����`��
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

#include "EnemyBullet.h"
#include "Affine.h"

void EnemyBullet::Initialize(const Vector3& position, const Vector3& velocity) {

	//���[���h�ϊ��f�[�^�̏�����
	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = { position.x, position.y, position.z };

	//�傫���ݒ�
	worldTransform_.scale_ = { 3.0f,3.0f,3.0f };

	//�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;

	//�A�t�B���ϊ�
	Affine::CreateAffine(worldTransform_);

	//�s��X�V
	worldTransform_.TransferMatrix();

	//���f��������
	model_.reset(Model::CreateFromOBJ("Enemy", true));

}

void EnemyBullet::Update() {
	//���W���ړ�������
	worldTransform_.translation_ += velocity_;

	//���Ԍo�߂Ńf�X
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	//�A�t�B���ϊ�
	Affine::CreateAffine(worldTransform_);
	//�s��X�V
	worldTransform_.TransferMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}
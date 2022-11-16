#include "Enemy.h"

void Enemy::Initialize()
{
	//�V�F�C�N���̈ړ�����
	shakeVal = 1.000f;
	//�o�ꎞ�̌o�ߎ���
	appearTimer = 0.0f;

	//���[���h�ϊ��f�[�^�̏�����
	worldTransform_.Initialize();

	//�������W
	worldTransform_.translation_ = { 0.0f,30.0f,0.0f };
	worldTransform_.scale_ = { 10.0f,10.0f,10.0f };

	//�A�t�B���ϊ�
	Affine::CreateAffine(worldTransform_);

	//�s��X�V
	worldTransform_.TransferMatrix();

	model_.reset(Model::CreateFromOBJ("Enemy", true));
}

void Enemy::Update()
{
	worldTransform_.translation_ = { 0.0f,0.0f,0.0f };

	//�A�t�B���ϊ�
	Affine::CreateAffine(worldTransform_);
	//�s��X�V
	worldTransform_.TransferMatrix();
}

void Enemy::Draw(ViewProjection* viewProjection)
{
	model_->Draw(worldTransform_, *viewProjection);
}

void Enemy::AppearMove()
{
	//Y0�܂ł�����艺�~
	if (worldTransform_.matWorld_.m[3][1] > 0.0f) {
		Affine::CreateMatTrans(worldTransform_.matWorld_, { 0.0f,-0.1f,0.0f });
	}
	else {
		//����������^�C�}�[�𑝂₷
		appearTimer++;
	}

	//��莞�Ԃ�������V�F�C�N
	if (appearTimer >= SHAKE_START) {

		//��莞�Ԃ�������V�F�C�N�ʂ������Â��炷
		if (shakeVal > 0 && appearTimer >= 150.0f) {
			shakeVal -= 0.01f;
		}

		Shake();
	}

	//�s��X�V
	worldTransform_.TransferMatrix();
}

void Enemy::Shake()
{
	worldTransform_.translation_ = { 0.0f,shakeVal,0.0f };
	Affine::CreateAffine(worldTransform_);

	shakeVal = -shakeVal;

}
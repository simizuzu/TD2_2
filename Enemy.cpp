#include "Enemy.h"
#include <cassert>

void Enemy::Initialize(Model* model)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	//���[���h�ϊ��f�[�^�̏�����
	worldTransform_.Initialize();

	//�������W
	worldTransform_.translation_ = { 0.0f,20.0f,0.0f };
	worldTransform_.scale_ = { 5.0f,5.0f,5.0f };

	//�A�t�B���ϊ�
	Affine::CreateAffine(worldTransform_);

	//�s��X�V
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
	//�s��X�V
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
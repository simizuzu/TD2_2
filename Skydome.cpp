#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;

	//���[���h�ϊ��f�[�^�̏�����
	worldTransform_.Initialize();

	Affine::CreateMatScale(worldTransform_.matWorld_, { 100.0f,100.0f,100.0f });

	//�s��X�V
	worldTransform_.TransferMatrix();
}

void Skydome::Draw(ViewProjection* viewProjection)
{
	model_->Draw(worldTransform_, *viewProjection);
}

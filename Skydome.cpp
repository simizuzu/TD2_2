#include "Skydome.h"

void Skydome::Initialize()
{
	// ���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	// �X�P�[����ݒ�
	worldTransform_.scale_ = { 100.0f,100.0f,100.0f };
	// �X�P�[�����O�s��
	Affine::CreateMatIdentity(worldTransform_.matWorld_);
	Affine::CreateMatScale(worldTransform_.matWorld_, worldTransform_.scale_);
	Affine::CreateMatRot(worldTransform_.matWorld_, { 0,0,0 });
	Affine::CreateMatTrans(worldTransform_.matWorld_, { 0,0,0 });
	// �s��̓]��
	worldTransform_.TransferMatrix();

	model_.reset(Model::CreateFromOBJ("skydome", true));
}

void Skydome::Draw(ViewProjection* viewProjection)
{
	model_->Draw(worldTransform_, *viewProjection);
}

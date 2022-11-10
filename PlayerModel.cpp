#include "PlayerModel.h"

void PlayerModel::Initialize()
{
	input_ = Input::GetInstance();

	// ���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	//���W�ݒ�
	worldTransform_.translation_ = { 0.0f,0.0f,-80.0f };
	// �X�P�[����ݒ�
	//worldTransform_.scale_ = { 10.0f,10.0f,10.0f };
	// �X�P�[�����O�s��
	
	Affine::CreateAffine(worldTransform_);

	// �s��̓]��
	worldTransform_.TransferMatrix();

	model_.reset(Model::CreateFromOBJ("Player", true));
}

void PlayerModel::Update()
{
	if (input_->PushKey(DIK_A)) {
		radian += 0.003f;
	}
	if (input_->PushKey(DIK_D)) {
		radian -= 0.003f;
	}

	if (radian >= 2.0f) {
		radian = 0.0f;
		//radian *= -1;
	}

	worldTransform_.translation_ = {
		-sin(3.14f * radian) * 80.0f,
		0.0f,
		-cos(3.14f * radian) * 80.0f
	};
	worldTransform_.rotation_ = {
		0.0f,
		radian * 3.14f,
		0.0f,
	};

	Affine::CreateAffine(worldTransform_);

	// �s��̓]��
	worldTransform_.TransferMatrix();

}

void PlayerModel::Draw(ViewProjection* viewProjection)
{
	model_->Draw(worldTransform_, *viewProjection);
}

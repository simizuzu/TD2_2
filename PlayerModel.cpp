#include "PlayerModel.h"

void PlayerModel::Initialize()
{
	input_ = Input::GetInstance();

	//���W�A��
	radian = 0.0f;

	// ���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	//���W�ݒ�
	worldTransform_.translation_ = { 0.0f,0.0f,-80.0f };
	//��]�p�ݒ�
	worldTransform_.rotation_ = { 0.0f,0.0f,0.0f };
	//�A�t�B���ϊ�
	Affine::CreateAffine(worldTransform_);

	// �s��̓]��
	worldTransform_.TransferMatrix();

	model_.reset(Model::CreateFromOBJ("Player", true));
}

void PlayerModel::Update()
{
	//�L�[���͂ňړ�
	if (input_->PushKey(DIK_A)) {
		radian += 0.003f;
	}
	if (input_->PushKey(DIK_D)) {
		radian -= 0.003f;
	}

	//���W�A����2�ȏ�Ȃ�0�ɖ߂�
	if (radian >= 2.0f) {
		radian = 0.0f;
	}

	//���ړ�
	worldTransform_.translation_ = {
		-sin(PI * radian) * 80.0f,
		0.0f,
		-cos(PI * radian) * 80.0f
	};
	//�ړ��ɍ��킹�ĉ�]
	worldTransform_.rotation_ = {
		0.0f,
		radian * PI,
		0.0f,
	};

	//�A�t�B���ϊ�
	Affine::CreateAffine(worldTransform_);

	// �s��̓]��
	worldTransform_.TransferMatrix();

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });

	//�L�����N�^�[�U������
	Attack();

	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update(worldTransform_);
	}

}

void PlayerModel::Draw(ViewProjection* viewProjection)
{
	model_->Draw(worldTransform_, *viewProjection);

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

}

void PlayerModel::Attack() {
	if (input_->PushKey(DIK_SPACE)) {

		playerPos = worldTransform_.translation_;

		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(kBulletSpeed * sinf(worldTransform_.rotation_.y), 0, kBulletSpeed * cosf(worldTransform_.rotation_.y));

		//�e�������A������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initilize(playerPos, velocity);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));

	}
}

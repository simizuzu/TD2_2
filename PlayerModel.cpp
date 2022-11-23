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

	model_ = Model::CreateFromOBJ("Player", true);

	std::unique_ptr<PlayerBits> newBit = std::make_unique<PlayerBits>();
	newBit->Initilize(worldTransform_.translation_);

	bits_.push_back(std::move(newBit));
	
	playerHp = 6;

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

	if (input_->PushKey(DIK_H) && isTimer == false)
	{
		isTimer = TRUE;
		playerHp -= 1;
	}

	if (isTimer == TRUE)
	{
		damegeTimer++;
		if (damegeTimer > 30)
		{
			damegeTimer = 0;
			isTimer = false;
		}
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
	bits_.remove_if([](std::unique_ptr<PlayerBits>& bits) { return bits->IsDead(); });

	//�L�����N�^�[�U������
	Attack();

	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update(worldTransform_,playerHp);
	}

	frontV = {worldTransform_.translation_.x - -sin(PI * radian) * 100.0f, worldTransform_.translation_.y - 0,worldTransform_.translation_.z - -cos(PI * radian) * 100.0f };

	sideV = { frontV.z,0,frontV.x };

	for (std::unique_ptr<PlayerBits>& bit : bits_) {
		bit->Update(worldTransform_, frontV,playerHp);
	}

}

void PlayerModel::Draw(ViewProjection* viewProjection)
{
	model_->Draw(worldTransform_, *viewProjection);

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

	for (std::unique_ptr<PlayerBits>& bit : bits_) {
		bit->Draw(viewProjection);
	}
}

void PlayerModel::Attack() {
	if (input_->PushKey(DIK_SPACE)) {

		bulletTimer++;

		playerPos = worldTransform_.translation_;

		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(kBulletSpeed * sinf(worldTransform_.rotation_.y), 0, kBulletSpeed * cosf(worldTransform_.rotation_.y));

		//�e�������A������
		if (bulletTimer == bulletTimerMax)
		{
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			newBullet->Initilize(playerPos, velocity);

			//�e��o�^����
			bullets_.push_back(std::move(newBullet));

			bulletTimer = 0;
		}

	}
}

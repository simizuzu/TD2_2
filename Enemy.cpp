#include "Enemy.h"
#include "Affine.h"

void Enemy::Initialize()
{
	input_ = Input::GetInstance();

	enemyHP = MAX_HP;
	//�V�F�C�N���̈ړ�����
	shakeVal = 0.500f;
	//�o�ꎞ�̌o�ߎ���
	appearTimer = 0.0f;

	//���[���h�ϊ��f�[�^�̏�����
	worldTransform_.Initialize();

	//�������W
	worldTransform_.translation_ = { 0.0f,30.0f,0.0f };
	worldTransform_.scale_ = { 10.0f,10.0f,10.0f };

	float radian = 2.0f;

	//�A�t�B���ϊ�
	Affine::CreateAffine(worldTransform_);

	//�s��X�V
	worldTransform_.TransferMatrix();
	//���f��������
	model_.reset(Model::CreateFromOBJ("Enemy", true));

	//�V�[���h������
	shield_.Initialize();
}

void Enemy::Update()
{
	if (input_->TriggerKey(DIK_S) && shield_.GetShieldHP2() <= 0) {
		enemyHP--;
	}

	//���˃^�C�}�[�J�E���g�_�E��
	fireTimer--;

	//�w�莞�ԂɒB����
	if (fireTimer <= 0 && shield_.GetShieldHP2() > 0) {
		//�e����
		Fire();
		//���˃^�C�}�[��������
		fireTimer = kFireInterval;
	}

	//�����O�ɃV�F�C�N
	if (shield_.GetRevivalTimer() > 0 && shield_.GetRevivalTimer() <= 50) {
		Shake();
	}
	else {
		shakeVal = 1.0f;
	}

	//�A�t�B���ϊ�
	Affine::CreateAffine(worldTransform_);
	//�s��X�V
	worldTransform_.TransferMatrix();

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->GetIsDead(); });

	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}
	//�V�[���h�X�V
	shield_.Update();
}

//�e����
void Enemy::Fire()
{
	radian += 0.055f;
	//���W�A����2�ȏ�Ȃ�0�ɖ߂�
	if (radian >= 20.0f) {
		radian = 0.0f;
	}

	////���L�����̃��[���h���W���擾
	Vector3 velocity = {
		sin(3.14f * radian),
		0.0f,
		cos(3.14f * radian),
	};

	//�x�N�g���̒������A�����ɍ��킹��
	velocity *= kBulletSpeed;

	//�e�𐶐����A������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(worldTransform_.translation_, velocity);

	//�e��o�^����
	bullets_.push_back(std::move(newBullet));
}


void Enemy::Draw(ViewProjection* viewProjection)
{
	model_->Draw(worldTransform_, *viewProjection);

	//�e�`��
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(*viewProjection);
	}
	//�V�[���h�`��
	shield_.Draw(*viewProjection);
}

//�o�ꎞ�̓���
void Enemy::AppearMove()
{
	if (input_->PushKey(DIK_SPACE)) {
		worldTransform_.translation_ = { 0.0f,0.0f,0.0f };
		//�A�t�B���ϊ�
		Affine::CreateAffine(worldTransform_);
		//�s��X�V
		worldTransform_.TransferMatrix();
	}

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

	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}
}

//���j���̓���
void Enemy::Defeat()
{

}

//�V�F�C�N
void Enemy::Shake()
{
	worldTransform_.translation_ = { 0.0f,shakeVal,0.0f };
	Affine::CreateAffine(worldTransform_);

	shakeVal = -shakeVal;

}
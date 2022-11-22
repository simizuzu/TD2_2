#include "Enemy.h"
#include "Affine.h"

Enemy::~Enemy()
{
	delete player_;
	delete shieldModel_;
	delete bulletModel_;
}

void Enemy::Initialize()
{
	input_ = Input::GetInstance();

	//�ϐ��Q������
	enemyHP = maxHP;
	shakeVal = 0.500f;
	appearTimer = 0.0f;
	defeatTimer = 0.0f;
	phaseTimer = 0;
	phaseNumber = 0;
	phase_ = Phase::rest;

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
	shieldModel_ = Model::CreateFromOBJ("Enemy", true);
	bulletModel_ = Model::CreateFromOBJ("Enemy", true);

	//�V�[���h������
	shield_.Initialize(shieldModel_);
}

void Enemy::Update()
{
	//���˃^�C�}�[�J�E���g�_�E��
	fireTimer--;
	//�t�F�C�Y�^�C�}�[
	phaseTimer++;

	if (shield_.GetShieldHP2() > 0) {
		switch (phase_)
		{
		case Phase::rest:
			Shake();

			//�̗�1/4�ȉ��Ŕ���
			if (enemyHP <= maxHP / 4)
			{
				phase_ = Phase::craziness;
			}
			else
			{
				//���b�o�����玟�̃t�F�C�Y��
				if (phaseTimer >= restTimer) {
					if (phaseNumber == 0) {
						phase_ = Phase::wholeAttack;
						phaseNumber = 1;
					}
					else {
						phase_ = Phase::pRelianceAttack;
						phaseNumber = 0;
					}

					phaseTimer = 0;
				}
			}


			break;
		case Phase::wholeAttack:

			//�w�莞�ԂɒB����
			if (fireTimer <= 0 && shield_.GetShieldHP2() > 0) {
				//�e����
				WholeFire();
				//���˃^�C�}�[��������
				fireTimer = kFireInterval;
			}

			//���b�o�����玟�̃t�F�C�Y��
			if (phaseTimer >= attackTimer) {
				phase_ = Phase::rest;
				phaseTimer = 0;
			}
			shakeVal = 1.0f;

			break;
		case Phase::pRelianceAttack:
			//�w�莞�ԂɒB����
			if (fireTimer <= 0 && shield_.GetShieldHP2() > 0) {
				//�e����
				PRelianceFire();
				//���˃^�C�}�[��������
				fireTimer = kFireInterval;
			}

			//���b�o�����玟�̃t�F�C�Y��
			if (phaseTimer >= attackTimer) {
				phase_ = Phase::rest;
				phaseTimer = 0;
			}
			shakeVal = 1.0f;

			break;
		case Phase::craziness:
			//�w�莞�ԂɒB����
			if (fireTimer <= 0 && shield_.GetShieldHP2() > 0) {
				//�e����
				PRelianceFire();
				WholeFire();
				//���˃^�C�}�[��������
				fireTimer = kFireInterval;
			}
			break;
		default:
			break;
		}
	}


	//�����O�ɃV�F�C�N
	if (shield_.GetRevivalTimer() > 0 && shield_.GetRevivalTimer() <= 50) {
		Shake();
	}
	else {

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

//�e����(�S���ʒe)
void Enemy::WholeFire()
{

	//���W�A����2�ȏ�Ȃ�0�ɖ߂�
	if (radian >= 2.0f) {
		radian = radianSpeed;
	}
	radian += radianSpeed;


	//�e�𐶐����A������
	for (int i = 0; i < bulletNum; i++) {
		//���x�ݒ�
		velocity = {
		 sin(3.14f * (radian + (2.0f / bulletNum * i))),
		 0.0f,
		 cos(3.14f * (radian + (2.0f / bulletNum * i))),
		};
		//�x�N�g���̒������A�����ɍ��킹��
		velocity *= kBulletSpeedA;

		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(bulletModel_, worldTransform_.translation_, velocity);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));
	}
}

//�e����(���@�ˑ��e)
void Enemy::PRelianceFire()
{
	//���L�����̃��[���h���W���擾
	Vector3 velocity = {
		player_->GetWorldTransform().matWorld_.m[3][0],
		player_->GetWorldTransform().matWorld_.m[3][1],
		player_->GetWorldTransform().matWorld_.m[3][2],
	};

	//�G�L�����̃��[���h���W���擾
	Vector3 enemyPos = {
		GetWorldTransform().matWorld_.m[3][0],
		GetWorldTransform().matWorld_.m[3][1],
		GetWorldTransform().matWorld_.m[3][2],
	};
	//�G�L���������L�����̍����x�N�g�������߂�
	velocity -= enemyPos;
	//�׃N�g���̐��K��
	velocity /= sqrt(pow(velocity.x, 2.0f) + pow(velocity.y, 2.0f) + pow(velocity.z, 2.0f));
	//�x�N�g���̒������A�����ɍ��킹��
	velocity *= kBulletSpeedB;

	//�e�𐶐����A������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(bulletModel_, worldTransform_.translation_, velocity);

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
	//�e������
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->BulletDeath();
	}
	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->GetIsDead(); });

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

}

//���j���̓���
void Enemy::DefeatMove()
{
	defeatTimer++;

	//��莞�Ԃ�������V�F�C�N�ʂ������Â��炷
	/*if (shakeVal > 0 && appearTimer >= 150.0f) {
		shakeVal -= 0.01f;
	}*/
	if (defeatTimer < 150.0f) {
		Shake();
	}
	else {
		worldTransform_.translation_.y -= 1.0f;
	}

	//�A�t�B���ϊ�
	Affine::CreateAffine(worldTransform_);
	//�s��X�V
	worldTransform_.TransferMatrix();
}

//�V�F�C�N
void Enemy::Shake()
{
	worldTransform_.translation_ = { 0.0f,shakeVal,0.0f };
	//�A�t�B���ϊ�
	Affine::CreateAffine(worldTransform_);
	//�s��X�V
	worldTransform_.TransferMatrix();

	shakeVal = -shakeVal;

}

void Enemy::OnCollision()
{
	if (shield_.GetShieldHP2() <= 0) {
		enemyHP--;
	}
	else {
		shield_.OnCollision();
	}
}
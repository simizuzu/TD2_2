#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerModel.h"
#include "PlayerBullet.h"
#include "EnemyBullet.h"
#include "EnemyShield.h"
#include <list>

//�V�F�C�N�J�n���鎞��
static const float SHAKE_START = 50.0f;

class Enemy
{
public:
	//�f�X�g���N�^
	~Enemy();

	//������
	void Initialize();

	//�X�V
	void Update();

	//�`��
	void Draw(ViewProjection* viewProjection);

	//�o�ꎞ�̓���
	void AppearMove();

	//�ޏꎞ�̓���
	void DefeatMove();

	//�e����(�S���ʒe)
	void WholeFire();

	//�e����(���@�ˑ��e)
	void PRelianceFire();

	//�V�F�C�N
	void Shake();

	//�q�b�g����
	void OnCollision();

	//�GHP�̃Q�b�^�[
	int GetEnemyHP() { return enemyHP; }

	//���[���h�s��̃Q�b�^�[
	WorldTransform GetWorldTransform() { return worldTransform_; }
	//�o�ꎞ�̌o�ߎ��Ԃ̃Q�b�^�[
	float GetAppearTimer() { return appearTimer; }

	//�e���X�g���擾
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	// setter
	void SetPlayer(PlayerModel* player) { player_ = player; }

private:
	//�s���t�F�[�Y
	enum class Phase {
		rest,			//�x�e
		wholeAttack,	//�S���ʍU��
		pRelianceAttack,//���@�ˑ��U��
		craziness		//�����U��
	};

	//�t�F�[�Y
	Phase phase_ = Phase::rest;

	Input* input_ = nullptr;
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	std::unique_ptr<Model> model_;
	//�V�[���h�̃��f��
	Model* shieldModel_;
	//�e�̃��f��
	Model* bulletModel_;

	//�e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//�V�[���h
	EnemyShield shield_;
	//���L����
	PlayerModel* player_ = nullptr;

	//�V�F�C�N���̈ړ�����
	float shakeVal = 0.500f;
	//�o�ꎞ�̌o�ߎ���
	float appearTimer = 0.0f;
	//���j���̌o�ߎ���
	float defeatTimer = 0.0f;

	//���˃^�C�}�[
	int32_t fireTimer = 0;
	//���ˊԊu
	int kFireInterval = 30;

	//�G�{�̂̍ő�HP
	const int maxHP = 100;
	//�G�{�̂�HP
	int enemyHP = maxHP;

	//�e�̑��x
	float kBulletSpeedA = 0.5f;
	float kBulletSpeedB = 1.5f;

	//���W�A��
	float radian = 0.0f;
	const float radianSpeed = 0.05f;
	//�e�̑��x
	Vector3 velocity;
	//��x�ɔ��˂���e�̐�
	const int bulletNum = 10;

	//�t�F�[�Y�ڍs�̃^�C�}�[
	int phaseTimer = 0;
	int phaseNumber = 0;
	//�x�e�̎���
	const int restTimer = 100;
	//�U���̎���
	const int attackTimer = 500;
};


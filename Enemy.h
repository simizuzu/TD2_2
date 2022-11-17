#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerModel.h"
#include "EnemyBullet.h"
#include "EnemyShield.h"
#include <list>

//�V�F�C�N�J�n���鎞��
static const float SHAKE_START = 50.0f;

class Enemy
{
public:
	//������
	void Initialize();

	//�X�V
	void Update();

	//�`��
	void Draw(ViewProjection* viewProjection);

	//�o�ꎞ�̓���
	void AppearMove();

	//�ޏꎞ�̓���
	void Defeat();

	//�e����
	void Fire();

	//�V�F�C�N
	void Shake();

	//�GHP�̃Q�b�^�[
	int GetEnemyHP() { return enemyHP; }

	//���[���h�s��̃Q�b�^�[
	WorldTransform GetWorldTransform() { return worldTransform_; }
	//�o�ꎞ�̌o�ߎ��Ԃ̃Q�b�^�[
	float GetAppearTimer() { return appearTimer; }

	static const int MAX_HP = 100;

	//// setter
	//void SetPlayer(PlayerModel* player) { player_ = player; }

	////���L����
	//PlayerModel* player_ = nullptr;

private:
	Input* input_ = nullptr;
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	std::unique_ptr<Model> model_;

	//�e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//�V�[���h
	EnemyShield shield_;

	//�V�F�C�N���̈ړ�����
	float shakeVal;
	//�o�ꎞ�̌o�ߎ���
	float appearTimer;

	//���˃^�C�}�[
	int32_t fireTimer = 0;
	//���ˊԊu
	int kFireInterval = 5;

	//�G�{�̂�HP
	int enemyHP = MAX_HP;

	//�e�̑��x
	float kBulletSpeed = 0.5f;

	//���W�A��
	float radian = 0.0f;

};


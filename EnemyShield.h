#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"

class EnemyShield
{
public:
	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void OnCollision();

	//�G�V�[���hHP�̃Q�b�^�[
	int GetShieldHP1() { return shieldHP1; }
	int GetShieldHP2() { return shieldHP2; }

	//�������Ԃ̃Q�b�^�[
	int GetRevivalTimer() { return revivalTimer; }

	//�V�[���h�S�̂̃��f���̐�
	static const int SHIELD_MODEL = 40;
	static const int SHIELD_MODEL_HALF = SHIELD_MODEL / 2;

	//�����܂ł̎���
	static const int REVIVAL_TIMER_MAX = 300;

private:
	Input* input_ = nullptr;

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_[SHIELD_MODEL];
	//���f��
	Model* model_[SHIELD_MODEL];

	//�V�[���hHP�̍ő�l
	static const int SHIELD_HP_MAX = 15;

	//�V�[���h�̗����ŏ�����
	const float DISTANCE_MIN = 20;

	const float size = 0.2f;
	const float removeSpeed = 5.0f;

	//���x
	float velocity = 0.005f;

	//�~����
	const float PI = 3.14159f;
	//���W�A��
	float radian;

	//�V�[���h��HP
	int shieldHP1 = SHIELD_HP_MAX;
	int shieldHP2 = SHIELD_HP_MAX;
	//�G����̋���
	float distance1 = DISTANCE_MIN * 10.0f;
	float distance2 = DISTANCE_MIN * 10.0f;

	//�_�E�����畜���܂ł̎���
	int revivalTimer = REVIVAL_TIMER_MAX;
};


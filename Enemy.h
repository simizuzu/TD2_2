#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Affine.h"

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

	//�V�F�C�N
	void Shake();

	//���[���h�s��̃Q�b�^�[
	WorldTransform GetWorldTransform() { return worldTransform_; }
	//�o�ꎞ�̌o�ߎ��Ԃ̃Q�b�^�[
	float GetAppearTimer() { return appearTimer; }

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	std::unique_ptr<Model> model_;

	//�V�F�C�N���̈ړ�����
	float shakeVal;
	//�o�ꎞ�̌o�ߎ���
	float appearTimer;


};


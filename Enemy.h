#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Affine.h"

class Enemy
{
public:
	//������
	void Initialize(Model* model);

	//�X�V
	void Update();

	//�`��
	void Draw(ViewProjection* viewProjection);

	//�V�F�C�N
	void Shake();

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;

	//�V�F�C�N���̈ړ�����
	float shakeX = 0.2f;
};


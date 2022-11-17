#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Input.h"
#include "MathMatrix.h"

class PlayerBits
{
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	void Initilize(const Vector3& position);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(WorldTransform worldTransform, Vector3 V);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(ViewProjection* viewProjection);

	bool IsDead() const { return isDead_; }

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransformBit[6];

	int bitMax = 6;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0;

	// ���f��
	std::unique_ptr<Model> model_;

	const float PI = 3.14159f;

	float rotaSpeed = 0;

	//�f�X�t���O
	bool isDead_ = false;

};

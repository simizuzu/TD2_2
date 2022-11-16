#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Affine.h"
#include "Input.h"

class PlayerModel
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection* viewProjection);

	//���[���h�s��̃Q�b�^�[
	WorldTransform GetWorldTransform() { return worldTransform_; }

private:
	const float PI = 3.14159f;

	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	// ���f��
	std::unique_ptr<Model> model_;
	
	Input* input_ = nullptr;

	//���W�A��
	float radian;
};
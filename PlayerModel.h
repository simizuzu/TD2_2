#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Affine.h"

class PlayerModel
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection* viewProjection);
private:
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	// ���f��
	std::unique_ptr<Model> model_;
};
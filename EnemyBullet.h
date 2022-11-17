#pragma once
#include "Model.h"
#include "WorldTransform.h"

class EnemyBullet
{
public:
	void Initialize(const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	//�Q�b�^�[
	bool GetIsDead() { return isDead_; }

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	std::unique_ptr<Model> model_;

	//���x
	Vector3 velocity_;
	//�f�X�t���O
	bool isDead_ = false;
	//�e�̎���
	static const int32_t kLifeTimer = 60 * 10;
	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTimer;


};


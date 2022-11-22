#pragma once
#include "Model.h"
#include "WorldTransform.h"

class EnemyBullet
{
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//�Q�b�^�[
	bool GetIsDead() { return isDead_; }

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_;

	//���x
	Vector3 velocity_;
	//�f�X�t���O
	bool isDead_ = false;
	//�e�̎���
	static const int32_t kLifeTimer = 60 * 10;
	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTimer;


};


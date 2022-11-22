#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Input.h"
#include "MathMatrix.h"

/// <summary>
/// ���L�����̒e
/// </summary>
class PlayerBullet
{
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	//void Initilize(Model* model, const Vector3& position, const Vector3& velocity);
	void Initilize(const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(WorldTransform worldTransform);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(ViewProjection* viewProjection);

	bool IsDead() const { return isDead_; }

	Vector3 BulletPos() const { return worldTransformBullet_.translation_; }

	Vector3 GetWorldPos();

	void OnCollision();

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransformBullet_;

	Vector3 worldTransformBulletPos_;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0;

	// ���f��
	std::unique_ptr<Model> model_;

	//���x
	Vector3 velocity_;

	//����<frm>
	static const int32_t kLifeTime = 60 * 1.5;

	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;

	//�f�X�t���O
	bool isDead_ = false;

};

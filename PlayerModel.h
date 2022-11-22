#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Affine.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "PlayerBits.h"

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

	void Attack();

	//�q�b�g����
	void OnCollision() { playerHp--; }

	//���[���h�s��̃Q�b�^�[
	WorldTransform GetWorldTransform() { return worldTransform_; }

	float GetPlayerHp() { return playerHp; }

	const std::list < std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	const std::list < std::unique_ptr<PlayerBits>>& GetBits() { return bits_; }

private:
	const float PI = 3.14159f;

	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	// ���f��
	std::unique_ptr<Model> model_;
	
	Input* input_ = nullptr;

	//���W�A��
	float radian;

	Vector3 playerPos;

	Vector3 frontV;
	Vector3 sideV;

	float bulletTimer = 0;
	float bulletTimerMax = 10;

	float playerHp = 6;

	float damegeTimer = 0;

	bool isTimer = false;

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	std::list<std::unique_ptr<PlayerBits>> bits_;
};
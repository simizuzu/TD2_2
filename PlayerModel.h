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
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(int Hp);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection* viewProjection);

	void Attack();

	//ヒット判定
	void OnCollision() { playerHp--; }

	float Dead()
	{
		return playerHp = 0;
	}

	void Finish(int enemyHp);

	//ワールド行列のゲッター
	WorldTransform GetWorldTransform() { return worldTransform_; }

	float GetPlayerHp() { return playerHp; }

	const std::list < std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	const std::list < std::unique_ptr<PlayerBits>>& GetBits() { return bits_; }

private:
	const float PI = 3.14159f;

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	//std::unique_ptr<Model> model_;
	Model* model_ = nullptr;
	
	Input* input_ = nullptr;

	//ラジアン
	float radian;

	Vector3 playerPos;

	Vector3 frontV;
	Vector3 sideV;

	float bulletTimer = 0;
	float bulletTimerMax = 10;

	float playerHp = 6;

	float damegeTimer = 0;

	bool isTimer = false;

	int enemyHp = 0;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	std::list<std::unique_ptr<PlayerBits>> bits_;
};
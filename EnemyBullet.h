#pragma once
#include "Model.h"
#include "WorldTransform.h"

class EnemyBullet
{
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//ゲッター
	bool GetIsDead() { return isDead_; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_;

	//速度
	Vector3 velocity_;
	//デスフラグ
	bool isDead_ = false;
	//弾の寿命
	static const int32_t kLifeTimer = 60 * 10;
	//デスタイマー
	int32_t deathTimer_ = kLifeTimer;


};


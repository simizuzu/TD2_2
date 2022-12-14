#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Input.h"
#include "MathMatrix.h"

/// <summary>
/// 自キャラの弾
/// </summary>
class PlayerBullet
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	//void Initilize(Model* model, const Vector3& position, const Vector3& velocity);
	void Initilize(const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(WorldTransform worldTransform, float hp, int enemyHp);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(ViewProjection* viewProjection);

	bool IsDead() const { return isDead_; }

	Vector3 BulletPos() const { return worldTransformBullet_.translation_; }

	Vector3 GetWorldPos();

	void Finish(int enemyHp);

	void OnCollision();

private:
	//ワールド変換データ
	WorldTransform worldTransformBullet_;

	Vector3 worldTransformBulletPos_;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// モデル
	//std::unique_ptr<Model> model_;
	Model* model_ = nullptr;

	//速度
	Vector3 velocity_;

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 1.5;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

	//デスフラグ
	bool isDead_ = false;

};

#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"

class EnemyShield
{
public:
	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void OnCollision();

	//敵シールドHPのゲッター
	int GetShieldHP1() { return shieldHP1; }
	int GetShieldHP2() { return shieldHP2; }

	//復活時間のゲッター
	int GetRevivalTimer() { return revivalTimer; }

	//シールド全体のモデルの数
	static const int SHIELD_MODEL = 40;
	static const int SHIELD_MODEL_HALF = SHIELD_MODEL / 2;

	//復活までの時間
	static const int REVIVAL_TIMER_MAX = 300;

private:
	Input* input_ = nullptr;

	//ワールド変換データ
	WorldTransform worldTransform_[SHIELD_MODEL];
	//モデル
	Model* model_[SHIELD_MODEL];

	//シールドHPの最大値
	static const int SHIELD_HP_MAX = 15;

	//シールドの離れる最小距離
	const float DISTANCE_MIN = 20;

	const float size = 0.2f;
	const float removeSpeed = 5.0f;

	//速度
	float velocity = 0.005f;

	//円周率
	const float PI = 3.14159f;
	//ラジアン
	float radian;

	//シールドのHP
	int shieldHP1 = SHIELD_HP_MAX;
	int shieldHP2 = SHIELD_HP_MAX;
	//敵からの距離
	float distance1 = DISTANCE_MIN * 10.0f;
	float distance2 = DISTANCE_MIN * 10.0f;

	//ダウンから復活までの時間
	int revivalTimer = REVIVAL_TIMER_MAX;
};


#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerModel.h"
#include "EnemyBullet.h"
#include "EnemyShield.h"
#include <list>

//シェイク開始する時間
static const float SHAKE_START = 50.0f;

class Enemy
{
public:
	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw(ViewProjection* viewProjection);

	//登場時の動き
	void AppearMove();

	//退場時の動き
	void Defeat();

	//弾発射
	void Fire();

	//シェイク
	void Shake();

	//敵HPのゲッター
	int GetEnemyHP() { return enemyHP; }

	//ワールド行列のゲッター
	WorldTransform GetWorldTransform() { return worldTransform_; }
	//登場時の経過時間のゲッター
	float GetAppearTimer() { return appearTimer; }

	static const int MAX_HP = 100;

	//// setter
	//void SetPlayer(PlayerModel* player) { player_ = player; }

	////自キャラ
	//PlayerModel* player_ = nullptr;

private:
	Input* input_ = nullptr;
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	std::unique_ptr<Model> model_;

	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//シールド
	EnemyShield shield_;

	//シェイク時の移動距離
	float shakeVal;
	//登場時の経過時間
	float appearTimer;

	//発射タイマー
	int32_t fireTimer = 0;
	//発射間隔
	int kFireInterval = 5;

	//敵本体のHP
	int enemyHP = MAX_HP;

	//弾の速度
	float kBulletSpeed = 0.5f;

	//ラジアン
	float radian = 0.0f;

};


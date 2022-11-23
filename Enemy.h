#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerModel.h"
#include "PlayerBullet.h"
#include "EnemyBullet.h"
#include "EnemyShield.h"
#include <list>

//シェイク開始する時間
static const float SHAKE_START = 50.0f;

class Enemy
{
public:
	//デストラクタ
	~Enemy();

	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw(ViewProjection* viewProjection);

	//登場時の動き
	void AppearMove();

	//退場時の動き
	void DefeatMove();

	//弾発射(全方位弾)
	void WholeFire();

	//弾発射(自機依存弾)
	void PRelianceFire();

	//シェイク
	void Shake();

	//ヒット判定
	void OnCollision();

	//敵HPのゲッター
	int GetEnemyHP() { return enemyHP; }

	//ワールド行列のゲッター
	WorldTransform GetWorldTransform() { return worldTransform_; }
	//登場時の経過時間のゲッター
	float GetAppearTimer() { return appearTimer; }

	//弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	// setter
	void SetPlayer(PlayerModel* player) { player_ = player; }

private:
	//行動フェーズ
	enum class Phase {
		rest,			//休憩
		wholeAttack,	//全方位攻撃
		pRelianceAttack,//自機依存攻撃
		craziness		//発狂攻撃
	};

	//フェーズ
	Phase phase_ = Phase::rest;

	Input* input_ = nullptr;
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	std::unique_ptr<Model> model_;
	//シールドのモデル
	Model* shieldModel_;
	//弾のモデル
	Model* bulletModel_;

	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//シールド
	EnemyShield shield_;
	//自キャラ
	PlayerModel* player_ = nullptr;

	//シェイク時の移動距離
	float shakeVal = 0.500f;
	//登場時の経過時間
	float appearTimer = 0.0f;
	//撃破時の経過時間
	float defeatTimer = 0.0f;

	//発射タイマー
	int32_t fireTimer = 0;
	//発射間隔
	int kFireInterval = 30;

	//敵本体の最大HP
	const int maxHP = 100;
	//敵本体のHP
	int enemyHP = maxHP;

	//弾の速度
	float kBulletSpeedA = 0.5f;
	float kBulletSpeedB = 1.5f;

	//ラジアン
	float radian = 0.0f;
	const float radianSpeed = 0.05f;
	//弾の速度
	Vector3 velocity;
	//一度に発射する弾の数
	const int bulletNum = 10;

	//フェーズ移行のタイマー
	int phaseTimer = 0;
	int phaseNumber = 0;
	//休憩の時間
	const int restTimer = 100;
	//攻撃の時間
	const int attackTimer = 500;
};


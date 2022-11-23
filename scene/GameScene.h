#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Skydome.h"
#include "Enemy.h"
#include "PlayerModel.h"
#include "MoveCamera.h"
#include "AudioManager.h"
#include "TextureManager.h"
#include "Sprite.h"

enum Scene {
	title,
	appear,
	game,
	defeat,
	gameover,
	clear,
	stop
};


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
  /// <summary>
  /// コンストクラタ
  /// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//当たり判定
	void CheckCollision();

	//自機
	PlayerModel* player_ = nullptr;

	//敵
	Enemy* enemy_ = nullptr;

	//天球
	Skydome* skydome_ = nullptr;

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//シーン
	Scene scene = title;

	//モデル
	Model* modelEnemy_ = nullptr;
	//天球のモデル
	Model* modelSkydome_ = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;
	//ビュープロジェクションの位置保存用
	Vector3 viewProjectionPos;
	//カメラの動き
	MoveCamera moveCamera_;

	// スプライト
	uint32_t titleTex = 0;
	uint32_t resultTex1 = 0;
	uint32_t resultTex2 = 0;
	std::unique_ptr<Sprite> spriteTitle;
	std::unique_ptr<Sprite> resultTitle1;
	std::unique_ptr<Sprite> resultTitle2;

	// サウンド
	uint32_t titleSound = 0;
	uint32_t gameSound = 0;
	uint32_t clearSound = 0;
	uint32_t overSound = 0;

	uint32_t checkTitle = 0;
	uint32_t checkGame = 0;
	uint32_t checkClear = 0;
	uint32_t checkOver = 0;

	int soundNowPlay = 0;
	int soundStop = 0;
	int bossHP;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};

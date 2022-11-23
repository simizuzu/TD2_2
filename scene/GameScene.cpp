﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete skydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	modelEnemy_ = Model::CreateFromOBJ("Enemy", true);
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	//自機の生成
	player_ = new PlayerModel();
	player_->Initialize();

	//敵生成
	enemy_ = new Enemy();
	enemy_->Initialize();

	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	//天球の生成
	skydome_ = new Skydome();
	//天球の初期化
	skydome_->Initialize(modelSkydome_);

	moveCamera_.Initialize();
	viewProjection_.Initialize();

	titleSound = audio_->LoadWave("title.mp3");
	gameSound = audio_->LoadWave("game.mp3");
	clearSound = audio_->LoadWave("result.mp3");
	overSound = audio_->LoadWave("result.mp3");

	checkTitle = audio_->PlayWave(titleSound, true);
}

void GameScene::Update() {

	switch (scene)
	{
	case title://タイトル画面
		audio_->StopWave(checkClear);
		audio_->StopWave(checkOver);

		//スペース押したらシーン切り替え
		if (input_->TriggerKey(DIK_SPACE)) {
			//初期化
			moveCamera_.Initialize();
			viewProjection_.Initialize();
			player_->Initialize();
			enemy_->Initialize();
			bossHP = enemy_->GetEnemyHP();

			audio_->StopWave(checkTitle);
			checkGame = audio_->PlayWave(gameSound, true);
			//シーン移動
			scene = appear;
		}

		break;

	case appear://ボス登場シーン

		if (soundNowPlay == true && soundStop == false)
		{
			soundNowPlay = false;
		}

		enemy_->AppearMove();

		moveCamera_.Appear(&viewProjection_, enemy_);

		if (moveCamera_.GetFlame3() >= MAX_FLAME) {
			scene = game;
		}

		if (input_->TriggerKey(DIK_SPACE)) {
			scene = game;
		}

		break;

	case game:


		bossHP = enemy_->GetEnemyHP();
		player_->Update(bossHP);
		enemy_->Update();

		//当たり判定
		CheckCollision();

		//カメラの位置
		viewProjection_.eye.x = player_->GetWorldTransform().translation_.x * 1.5f;
		viewProjection_.eye.y = 20.0f;
		viewProjection_.eye.z = player_->GetWorldTransform().translation_.z * 1.5f;
		viewProjection_.target.x = 0.0f;
		viewProjection_.target.y = 0.0f;
		viewProjection_.target.z = 0.0f;

		if (player_->GetPlayerHp() <= 0) {
			audio_->StopWave(checkGame);
			checkClear = audio_->PlayWave(overSound, true);
			scene = gameover;
		}

		//ボスを倒したらシーンチェンジ
		if (enemy_->GetEnemyHP() <= 0) {
			moveCamera_.Initialize();
			scene = defeat;
		}

		//ビュープロジェクションの位置を保存
		viewProjectionPos = viewProjection_.eye;

		break;

	case defeat:
		enemy_->DefeatMove();

		moveCamera_.Defeat(&viewProjection_, viewProjectionPos, enemy_);

		//SPACEキーでリザルトへ
		if (input_->TriggerKey(DIK_SPACE)) {
			audio_->StopWave(checkGame);
			checkOver = audio_->PlayWave(clearSound, true);
			scene = clear;
		}
		break;

	case clear:
		player_->Dead();
		enemy_->Dead();
		bossHP = enemy_->GetEnemyHP();
		player_->Finish(bossHP);
		if (input_->TriggerKey(DIK_SPACE)) {
			checkTitle = audio_->PlayWave(titleSound, true);
			scene = title;
		}
		break;

	case gameover:
		player_->Dead();
		enemy_->Dead();
		bossHP = enemy_->GetEnemyHP();
		player_->Finish(bossHP);
		if (input_->TriggerKey(DIK_SPACE)) {
			checkTitle = audio_->PlayWave(titleSound, true);
			scene = title;
		}
		break;
	}

	viewProjection_.UpdateMatrix();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//player_->Draw(&viewProjection_);
	switch (scene)
	{
	case title:

		break;

	case appear:
		player_->Draw(&viewProjection_);
		enemy_->Draw(&viewProjection_);
		break;

	case game:
		player_->Draw(&viewProjection_);
		enemy_->Draw(&viewProjection_);
		break;

	case defeat:
		player_->Draw(&viewProjection_);
		enemy_->Draw(&viewProjection_);
		break;

	}
	skydome_->Draw(&viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	switch (scene)
	{
	case title:
		//float num = 60.0f;

		break;

	case appear:

		break;

	case game:

		break;

	case defeat:

		break;

	}

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();


#pragma endregion
}

void GameScene::CheckCollision()
{
	//判定対象AとBの座標
	Vector3 posA, posB;

	//敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();
	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();

	/*敵弾とプレイヤーの当たり判定*/
	//自キャラの座標
	posA = {
		player_->GetWorldTransform().matWorld_.m[3][0],
		player_->GetWorldTransform().matWorld_.m[3][1],
		player_->GetWorldTransform().matWorld_.m[3][2],
	};

	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();

		//座標AとBの距離を求める
		float distance =
			pow(posA.x - posB.x, 2.0f) + pow(posA.y - posB.y, 2.0f) + pow(posA.z - posB.z, 2.0f);

		//当たり判定の半径を設定
		float bulletRadian = 3.0f;
		float playerRadian = 1.0f;

		float collision = pow(bulletRadian + playerRadian, 2.0f);

		//球と球の交差判定
		if (distance <= collision) {
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
			player_->OnCollision();
		}
	}

	/*自弾と敵の当たり判定*/
	//敵キャラの座標
	posA = {
		enemy_->GetWorldTransform().matWorld_.m[3][0],
		enemy_->GetWorldTransform().matWorld_.m[3][1],
		enemy_->GetWorldTransform().matWorld_.m[3][2],
	};

	//敵キャラと自弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//自弾の座標
		posB = bullet->GetWorldPos();

		//座標AとBの距離を求める
		float distance =
			pow(posA.x - posB.x, 2.0f) + pow(posA.y - posB.y, 2.0f) + pow(posA.z - posB.z, 2.0f);

		//当たり判定の半径を設定
		float bulletRadian = 0.1f;
		float enemyRadian = 5.0f;

		float collision = pow(bulletRadian + enemyRadian, 2.0f);

		//球と球の交差判定
		if (distance <= collision) {
			//自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
			enemy_->OnCollision();
		}
	}
}


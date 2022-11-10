#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

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

	//天球の生成
	skydome_ = new Skydome();
	//天球の初期化
	skydome_->Initialize(modelSkydome_);

	moveCamera_.Initialize();
	viewProjection_.Initialize();

}

void GameScene::Update() {

	switch (scene)
	{
	case title://タイトル画面

		//スペース押したらシーン切り替え
		if (input_->TriggerKey(DIK_SPACE)) {
			scene = appear;
		}

		break;

	case appear://ボス登場シーン

		enemy_->AppearMove();

		moveCamera_.Appear(&viewProjection_, enemy_);

		if (input_->TriggerKey(DIK_SPACE)) {
			scene = game;
		}

		debugText_->SetPos(20.0f, 20.0f);
		debugText_->Printf("%f,%f,%f", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
		debugText_->SetPos(20.0f, 40.0f);
		debugText_->Printf("%f,%f,%f", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);

		break;

	case game:
		//if (input_->PushKey(DIK_A)) {
		//	radian += 0.003f;
		//}
		//if (input_->PushKey(DIK_D)) {
		//	radian -= 0.003f;
		//}

		//if (radian >= 2.0f) {
		//	radian = 0.0f;
		//}

		player_->Update();
		enemy_->Update();

		viewProjection_.eye.x = player_->GetWorldTransform().translation_.x * 1.5f;
		viewProjection_.eye.z = player_->GetWorldTransform().translation_.z * 1.5f;

		debugText_->SetPos(20.0f, 20.0f);
		debugText_->Printf("%f,%f,%f", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
		debugText_->SetPos(20.0f, 40.0f);
		debugText_->Printf("%f,%f,%f", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);

		if (input_->PushKey(DIK_W)) {
			viewProjection_.eye.y = 30.0f;
			viewProjection_.target.x = player_->GetWorldTransform().translation_.x * 0.5f;
			viewProjection_.target.z = player_->GetWorldTransform().translation_.z * 0.5f;
		}
		else {
			viewProjection_.target.x = 0.0f;
			viewProjection_.target.z = 0.0f;
			viewProjection_.eye.y = 10.0f;
		}

		viewProjection_.target.y = 0.0f;
		break;

	case pause:
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

	case pause:
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

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();


#pragma endregion
}

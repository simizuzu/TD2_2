#include "PlayerModel.h"

void PlayerModel::Initialize()
{
	input_ = Input::GetInstance();

	//ラジアン
	radian = 0.0f;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	//座標設定
	worldTransform_.translation_ = { 0.0f,0.0f,-80.0f };
	//回転角設定
	worldTransform_.rotation_ = { 0.0f,0.0f,0.0f };
	//アフィン変換
	Affine::CreateAffine(worldTransform_);

	// 行列の転送
	worldTransform_.TransferMatrix();

	model_.reset(Model::CreateFromOBJ("Player", true));
}

void PlayerModel::Update()
{
	//キー入力で移動
	if (input_->PushKey(DIK_A)) {
		radian += 0.003f;
	}
	if (input_->PushKey(DIK_D)) {
		radian -= 0.003f;
	}

	//ラジアンが2以上なら0に戻す
	if (radian >= 2.0f) {
		radian = 0.0f;
	}

	//実移動
	worldTransform_.translation_ = {
		-sin(PI * radian) * 80.0f,
		0.0f,
		-cos(PI * radian) * 80.0f
	};
	//移動に合わせて回転
	worldTransform_.rotation_ = {
		0.0f,
		radian * PI,
		0.0f,
	};

	//アフィン変換
	Affine::CreateAffine(worldTransform_);

	// 行列の転送
	worldTransform_.TransferMatrix();

}

void PlayerModel::Draw(ViewProjection* viewProjection)
{
	model_->Draw(worldTransform_, *viewProjection);
}

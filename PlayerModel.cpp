#include "PlayerModel.h"

void PlayerModel::Initialize()
{
	input_ = Input::GetInstance();

	// ワールド変換の初期化
	worldTransform_.Initialize();
	//座標設定
	worldTransform_.translation_ = { 0.0f,0.0f,-80.0f };
	// スケールを設定
	//worldTransform_.scale_ = { 10.0f,10.0f,10.0f };
	// スケーリング行列
	
	Affine::CreateAffine(worldTransform_);

	// 行列の転送
	worldTransform_.TransferMatrix();

	model_.reset(Model::CreateFromOBJ("Player", true));
}

void PlayerModel::Update()
{
	if (input_->PushKey(DIK_A)) {
		radian += 0.003f;
	}
	if (input_->PushKey(DIK_D)) {
		radian -= 0.003f;
	}

	if (radian >= 2.0f) {
		radian = 0.0f;
		//radian *= -1;
	}

	worldTransform_.translation_ = {
		-sin(3.14f * radian) * 80.0f,
		0.0f,
		-cos(3.14f * radian) * 80.0f
	};
	worldTransform_.rotation_ = {
		0.0f,
		radian * 3.14f,
		0.0f,
	};

	Affine::CreateAffine(worldTransform_);

	// 行列の転送
	worldTransform_.TransferMatrix();

}

void PlayerModel::Draw(ViewProjection* viewProjection)
{
	model_->Draw(worldTransform_, *viewProjection);
}

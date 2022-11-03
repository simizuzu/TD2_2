#include "Skydome.h"

void Skydome::Initialize()
{
	// ワールド変換の初期化
	worldTransform_.Initialize();
	// スケールを設定
	worldTransform_.scale_ = { 100.0f,100.0f,100.0f };
	// スケーリング行列
	Affine::CreateMatIdentity(worldTransform_.matWorld_);
	Affine::CreateMatScale(worldTransform_.matWorld_, worldTransform_.scale_);
	Affine::CreateMatRot(worldTransform_.matWorld_, { 0,0,0 });
	Affine::CreateMatTrans(worldTransform_.matWorld_, { 0,0,0 });
	// 行列の転送
	worldTransform_.TransferMatrix();

	model_.reset(Model::CreateFromOBJ("skydome", true));
}

void Skydome::Draw(ViewProjection* viewProjection)
{
	model_->Draw(worldTransform_, *viewProjection);
}

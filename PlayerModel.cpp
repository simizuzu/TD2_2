#include "PlayerModel.h"

void PlayerModel::Initialize()
{
	// ワールド変換の初期化
	worldTransform_.Initialize();
	// スケールを設定
	worldTransform_.scale_ = { 10.0f,10.0f,10.0f };
	// スケーリング行列
	Affine::CreateMatIdentity(worldTransform_.matWorld_);
	Affine::CreateMatScale(worldTransform_.matWorld_, worldTransform_.scale_);
	Affine::CreateMatRot(worldTransform_.matWorld_, { 0,0,0 });
	Affine::CreateMatTrans(worldTransform_.matWorld_, { 0,0,0 });
	// 行列の転送
	worldTransform_.TransferMatrix();

	model_.reset(Model::CreateFromOBJ("Player", true));
}

void PlayerModel::Draw(ViewProjection* viewProjection)
{
	model_->Draw(worldTransform_, *viewProjection);
}

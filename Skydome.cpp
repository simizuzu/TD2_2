#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model) {
	// NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	//ワールド変換データの初期化
	worldTransform_.Initialize();

	Affine::CreateMatScale(worldTransform_.matWorld_, { 100.0f,100.0f,100.0f });

	//行列更新
	worldTransform_.TransferMatrix();
}

void Skydome::Draw(ViewProjection* viewProjection)
{
	model_->Draw(worldTransform_, *viewProjection);
}

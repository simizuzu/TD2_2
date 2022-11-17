#include "Enemy.h"
#include "Affine.h"

void Enemy::Initialize()
{
	input_ = Input::GetInstance();

	enemyHP = MAX_HP;
	//シェイク時の移動距離
	shakeVal = 0.500f;
	//登場時の経過時間
	appearTimer = 0.0f;

	//ワールド変換データの初期化
	worldTransform_.Initialize();

	//初期座標
	worldTransform_.translation_ = { 0.0f,30.0f,0.0f };
	worldTransform_.scale_ = { 10.0f,10.0f,10.0f };

	float radian = 2.0f;

	//アフィン変換
	Affine::CreateAffine(worldTransform_);

	//行列更新
	worldTransform_.TransferMatrix();
	//モデル初期化
	model_.reset(Model::CreateFromOBJ("Enemy", true));

	//シールド初期化
	shield_.Initialize();
}

void Enemy::Update()
{
	if (input_->TriggerKey(DIK_S) && shield_.GetShieldHP2() <= 0) {
		enemyHP--;
	}

	//発射タイマーカウントダウン
	fireTimer--;

	//指定時間に達した
	if (fireTimer <= 0 && shield_.GetShieldHP2() > 0) {
		//弾発射
		Fire();
		//発射タイマーを初期化
		fireTimer = kFireInterval;
	}

	//復活前にシェイク
	if (shield_.GetRevivalTimer() > 0 && shield_.GetRevivalTimer() <= 50) {
		Shake();
	}
	else {
		shakeVal = 1.0f;
	}

	//アフィン変換
	Affine::CreateAffine(worldTransform_);
	//行列更新
	worldTransform_.TransferMatrix();

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->GetIsDead(); });

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}
	//シールド更新
	shield_.Update();
}

//弾発射
void Enemy::Fire()
{
	radian += 0.055f;
	//ラジアンが2以上なら0に戻す
	if (radian >= 20.0f) {
		radian = 0.0f;
	}

	////自キャラのワールド座標を取得
	Vector3 velocity = {
		sin(3.14f * radian),
		0.0f,
		cos(3.14f * radian),
	};

	//ベクトルの長さを、速さに合わせる
	velocity *= kBulletSpeed;

	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(worldTransform_.translation_, velocity);

	//弾を登録する
	bullets_.push_back(std::move(newBullet));
}


void Enemy::Draw(ViewProjection* viewProjection)
{
	model_->Draw(worldTransform_, *viewProjection);

	//弾描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(*viewProjection);
	}
	//シールド描画
	shield_.Draw(*viewProjection);
}

//登場時の動き
void Enemy::AppearMove()
{
	if (input_->PushKey(DIK_SPACE)) {
		worldTransform_.translation_ = { 0.0f,0.0f,0.0f };
		//アフィン変換
		Affine::CreateAffine(worldTransform_);
		//行列更新
		worldTransform_.TransferMatrix();
	}

	//Y0までゆっくり下降
	if (worldTransform_.matWorld_.m[3][1] > 0.0f) {
		Affine::CreateMatTrans(worldTransform_.matWorld_, { 0.0f,-0.1f,0.0f });
	}
	else {
		//到着したらタイマーを増やす
		appearTimer++;
	}

	//一定時間たったらシェイク
	if (appearTimer >= SHAKE_START) {

		//一定時間たったらシェイク量を少しづつ減らす
		if (shakeVal > 0 && appearTimer >= 150.0f) {
			shakeVal -= 0.01f;
		}

		Shake();
	}

	//行列更新
	worldTransform_.TransferMatrix();

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}
}

//撃破時の動き
void Enemy::Defeat()
{

}

//シェイク
void Enemy::Shake()
{
	worldTransform_.translation_ = { 0.0f,shakeVal,0.0f };
	Affine::CreateAffine(worldTransform_);

	shakeVal = -shakeVal;

}
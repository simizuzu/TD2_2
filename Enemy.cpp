#include "Enemy.h"
#include "Affine.h"

Enemy::~Enemy()
{
	delete player_;
	delete shieldModel_;
	delete bulletModel_;
}

void Enemy::Initialize()
{
	input_ = Input::GetInstance();

	//変数群初期化
	enemyHP = maxHP;
	shakeVal = 0.500f;
	appearTimer = 0.0f;
	defeatTimer = 0.0f;
	phaseTimer = 0;
	phaseNumber = 0;
	phase_ = Phase::rest;

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
	shieldModel_ = Model::CreateFromOBJ("Enemy", true);
	bulletModel_ = Model::CreateFromOBJ("Enemy", true);

	//シールド初期化
	shield_.Initialize(shieldModel_);
}

void Enemy::Update()
{
	//発射タイマーカウントダウン
	fireTimer--;
	//フェイズタイマー
	phaseTimer++;

	if (shield_.GetShieldHP2() > 0) {
		switch (phase_)
		{
		case Phase::rest:
			Shake();

			//体力1/4以下で発狂
			if (enemyHP <= maxHP / 4)
			{
				phase_ = Phase::craziness;
			}
			else
			{
				//数秒経ったら次のフェイズへ
				if (phaseTimer >= restTimer) {
					if (phaseNumber == 0) {
						phase_ = Phase::wholeAttack;
						phaseNumber = 1;
					}
					else {
						phase_ = Phase::pRelianceAttack;
						phaseNumber = 0;
					}

					phaseTimer = 0;
				}
			}


			break;
		case Phase::wholeAttack:

			//指定時間に達した
			if (fireTimer <= 0 && shield_.GetShieldHP2() > 0) {
				//弾発射
				WholeFire();
				//発射タイマーを初期化
				fireTimer = kFireInterval;
			}

			//数秒経ったら次のフェイズへ
			if (phaseTimer >= attackTimer) {
				phase_ = Phase::rest;
				phaseTimer = 0;
			}
			shakeVal = 1.0f;

			break;
		case Phase::pRelianceAttack:
			//指定時間に達した
			if (fireTimer <= 0 && shield_.GetShieldHP2() > 0) {
				//弾発射
				PRelianceFire();
				//発射タイマーを初期化
				fireTimer = kFireInterval;
			}

			//数秒経ったら次のフェイズへ
			if (phaseTimer >= attackTimer) {
				phase_ = Phase::rest;
				phaseTimer = 0;
			}
			shakeVal = 1.0f;

			break;
		case Phase::craziness:
			//指定時間に達した
			if (fireTimer <= 0 && shield_.GetShieldHP2() > 0) {
				//弾発射
				PRelianceFire();
				WholeFire();
				//発射タイマーを初期化
				fireTimer = kFireInterval;
			}
			break;
		default:
			break;
		}
	}


	//復活前にシェイク
	if (shield_.GetRevivalTimer() > 0 && shield_.GetRevivalTimer() <= 50) {
		Shake();
	}
	else {

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

//弾発射(全方位弾)
void Enemy::WholeFire()
{

	//ラジアンが2以上なら0に戻す
	if (radian >= 2.0f) {
		radian = radianSpeed;
	}
	radian += radianSpeed;


	//弾を生成し、初期化
	for (int i = 0; i < bulletNum; i++) {
		//速度設定
		velocity = {
		 sin(3.14f * (radian + (2.0f / bulletNum * i))),
		 0.0f,
		 cos(3.14f * (radian + (2.0f / bulletNum * i))),
		};
		//ベクトルの長さを、速さに合わせる
		velocity *= kBulletSpeedA;

		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(bulletModel_, worldTransform_.translation_, velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));
	}
}

//弾発射(自機依存弾)
void Enemy::PRelianceFire()
{
	//自キャラのワールド座標を取得
	Vector3 velocity = {
		player_->GetWorldTransform().matWorld_.m[3][0],
		player_->GetWorldTransform().matWorld_.m[3][1],
		player_->GetWorldTransform().matWorld_.m[3][2],
	};

	//敵キャラのワールド座標を取得
	Vector3 enemyPos = {
		GetWorldTransform().matWorld_.m[3][0],
		GetWorldTransform().matWorld_.m[3][1],
		GetWorldTransform().matWorld_.m[3][2],
	};
	//敵キャラ→自キャラの差分ベクトルを求める
	velocity -= enemyPos;
	//べクトルの正規化
	velocity /= sqrt(pow(velocity.x, 2.0f) + pow(velocity.y, 2.0f) + pow(velocity.z, 2.0f));
	//ベクトルの長さを、速さに合わせる
	velocity *= kBulletSpeedB;

	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(bulletModel_, worldTransform_.translation_, velocity);

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
	//弾を消す
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->BulletDeath();
	}
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->GetIsDead(); });

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

}

//撃破時の動き
void Enemy::DefeatMove()
{
	defeatTimer++;

	//一定時間たったらシェイク量を少しづつ減らす
	/*if (shakeVal > 0 && appearTimer >= 150.0f) {
		shakeVal -= 0.01f;
	}*/
	if (defeatTimer < 150.0f) {
		Shake();
	}
	else {
		worldTransform_.translation_.y -= 1.0f;
	}

	//アフィン変換
	Affine::CreateAffine(worldTransform_);
	//行列更新
	worldTransform_.TransferMatrix();
}

//シェイク
void Enemy::Shake()
{
	worldTransform_.translation_ = { 0.0f,shakeVal,0.0f };
	//アフィン変換
	Affine::CreateAffine(worldTransform_);
	//行列更新
	worldTransform_.TransferMatrix();

	shakeVal = -shakeVal;

}

void Enemy::OnCollision()
{
	if (shield_.GetShieldHP2() <= 0) {
		enemyHP--;
	}
	else {
		shield_.OnCollision();
	}
}
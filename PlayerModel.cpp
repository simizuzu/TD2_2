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

	model_ = Model::CreateFromOBJ("Player", true);

	std::unique_ptr<PlayerBits> newBit = std::make_unique<PlayerBits>();
	newBit->Initilize(worldTransform_.translation_);

	bits_.push_back(std::move(newBit));
	
	playerHp = 6;

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

	if (input_->PushKey(DIK_H) && isTimer == false)
	{
		isTimer = TRUE;
		playerHp -= 1;
	}

	if (isTimer == TRUE)
	{
		damegeTimer++;
		if (damegeTimer > 30)
		{
			damegeTimer = 0;
			isTimer = false;
		}
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

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });
	bits_.remove_if([](std::unique_ptr<PlayerBits>& bits) { return bits->IsDead(); });

	//キャラクター攻撃処理
	Attack();

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update(worldTransform_,playerHp);
	}

	frontV = {worldTransform_.translation_.x - -sin(PI * radian) * 100.0f, worldTransform_.translation_.y - 0,worldTransform_.translation_.z - -cos(PI * radian) * 100.0f };

	sideV = { frontV.z,0,frontV.x };

	for (std::unique_ptr<PlayerBits>& bit : bits_) {
		bit->Update(worldTransform_, frontV,playerHp);
	}

}

void PlayerModel::Draw(ViewProjection* viewProjection)
{
	model_->Draw(worldTransform_, *viewProjection);

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

	for (std::unique_ptr<PlayerBits>& bit : bits_) {
		bit->Draw(viewProjection);
	}
}

void PlayerModel::Attack() {
	if (input_->PushKey(DIK_SPACE)) {

		bulletTimer++;

		playerPos = worldTransform_.translation_;

		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(kBulletSpeed * sinf(worldTransform_.rotation_.y), 0, kBulletSpeed * cosf(worldTransform_.rotation_.y));

		//弾生成し、初期化
		if (bulletTimer == bulletTimerMax)
		{
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			newBullet->Initilize(playerPos, velocity);

			//弾を登録する
			bullets_.push_back(std::move(newBullet));

			bulletTimer = 0;
		}

	}
}

#include "EnemyShield.h"
#include "Affine.h"

void EnemyShield::Initialize(Model* model) {
	input_ = Input::GetInstance();

	//変数初期化
	//シールドのHP
	shieldHP1 = SHIELD_HP_MAX;
	shieldHP2 = SHIELD_HP_MAX;
	distance1 = DISTANCE_MIN * 10.0f;
	distance2 = DISTANCE_MIN * 10.0f;
	revivalTimer = REVIVAL_TIMER_MAX;
	radian = 0.0f;

	for (int i = 0; i < SHIELD_MODEL; i++) {
		//ワールド変換データの初期化
		worldTransform_[i].Initialize();

		//モデル初期化
		model_[i] = model;

		//初期座標設定
		if (i < SHIELD_MODEL_HALF) {
			worldTransform_[i].translation_ = {
				-sin(PI * (radian + (2.0f / SHIELD_MODEL_HALF * i))) * distance2,
				-sin(PI * (radian + (2.0f / SHIELD_MODEL_HALF * i))) * distance2,
				-cos(PI * (radian + (2.0f / SHIELD_MODEL_HALF * i))) * distance2
			};
		}
		else {
			worldTransform_[i].translation_ = {
				 sin(PI * (radian + (2.0f / SHIELD_MODEL_HALF * i))) * distance1,
				-sin(PI * (radian + (2.0f / SHIELD_MODEL_HALF * i))) * distance1,
				-cos(PI * (radian + (2.0f / SHIELD_MODEL_HALF * i))) * distance1
			};
		}

		//アフィン変換
		Affine::CreateAffine(worldTransform_[i]);

		//行列更新
		worldTransform_[i].TransferMatrix();

	}

}

void EnemyShield::Update() {

	//キー入力でHP変動
	if (input_->TriggerKey(DIK_S)) {
		if (shieldHP1 >= 0) {
			shieldHP1--;
		}
		else if (shieldHP2 >= 0) {
			shieldHP2--;
		}
	}
	/*if (input_->TriggerKey(DIK_X)) {
		shieldHP1 = SHIELD_HP_MAX;
		shieldHP2 = SHIELD_HP_MAX;
	}*/

	//シールドのHPが0になったら距離を離す
	if (shieldHP1 <= 0 && distance1 <= DISTANCE_MIN * 10.0f) {
		distance1 += removeSpeed;
	}
	else if (shieldHP1 > 0 && distance1 > DISTANCE_MIN) {
		distance1 -= removeSpeed;
	}

	if (shieldHP2 <= 0 && distance2 <= DISTANCE_MIN * 10.0f) {
		distance2 += removeSpeed;
	}
	else if (shieldHP2 > 0 && distance2 > DISTANCE_MIN) {
		distance2 -= removeSpeed;
	}

	//シールドのHPが0になったら復活タイマースタート
	if (shieldHP2 <= 0) {
		revivalTimer--;
	}
	//タイマーが0になったらシールド復活
	if (revivalTimer <= 0) {
		shieldHP1 = SHIELD_HP_MAX;
		shieldHP2 = SHIELD_HP_MAX;
		revivalTimer = REVIVAL_TIMER_MAX;
	}

	radian += velocity;
	//ラジアンが2以上なら0に戻す
	if (radian >= 2.0f) {
		radian = 0.0f;
	}

	for (int i = 0; i < SHIELD_MODEL; i++) {

		//実移動
		if (i < SHIELD_MODEL_HALF) {
			worldTransform_[i].translation_ = {
				-sin(PI * (radian * 3 + (2.0f / SHIELD_MODEL_HALF * i))) * distance2,
				-sin(PI * (radian * 1 + (2.0f / SHIELD_MODEL_HALF * i))) * distance2,
				-cos(PI * (radian * 3 + (2.0f / SHIELD_MODEL_HALF * i))) * distance2
			};
		}
		else {
			worldTransform_[i].translation_ = {
				 sin(PI * (radian * 3 + (2.0f / SHIELD_MODEL_HALF * i))) * distance1,
				-sin(PI * (radian * 1 + (2.0f / SHIELD_MODEL_HALF * i))) * distance1,
				-cos(PI * (radian * 3 + (2.0f / SHIELD_MODEL_HALF * i))) * distance1
			};
		}

		//HPに合わせて大きさ変更
		if (shieldHP1 > 0 && i >= SHIELD_MODEL_HALF) {
			worldTransform_[i].scale_ = {
				shieldHP1 * size + 1.0f,
				shieldHP1 * size + 1.0f,
				shieldHP1 * size + 1.0f,
			};
		}
		else if (shieldHP2 > 0 && i < SHIELD_MODEL_HALF) {
			worldTransform_[i].scale_ = {
				shieldHP2 * size + 1.0f,
				shieldHP2 * size + 1.0f,
				shieldHP2 * size + 1.0f,
			};
		}

		//アフィン変換
		Affine::CreateAffine(worldTransform_[i]);

		// 行列の転送
		worldTransform_[i].TransferMatrix();
	}

}

void EnemyShield::Draw(const ViewProjection& viewProjection) {
	for (int i = 0; i < SHIELD_MODEL; i++) {
		model_[i]->Draw(worldTransform_[i], viewProjection);
	}
}
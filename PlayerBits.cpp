#include "PlayerBits.h"
#include <cassert>

void PlayerBits::Initilize(const Vector3& position) {

	//テクスチャ読み込み
	model_ = Model::CreateFromOBJ("Heart", true);

	for (int i = 0; i < bitMax; i++)
	{
		worldTransformBit[i].rotation_ = { 0.0f, 0.0f, 0.0f };
		worldTransformBit[i].translation_ = position;

		worldTransformBit[i].scale_ = { 0.5, 0.5, 0.5 };
		worldTransformBit[i].Initialize();
	}

}

void PlayerBits::Update(WorldTransform worldTransform,Vector3 V, float Hp) {

	bitMax = Hp;

	rotaSpeed -= 0.01;

	float r = 6.0f;

	for (int i = 0; i < bitMax; i++)
	{
		worldTransformBit[i].translation_ = worldTransform.translation_;

		worldTransformBit[0].translation_.x += V.z * 0.2f;
		worldTransformBit[0].translation_.y += r;
		worldTransformBit[0].translation_.z -= V.x * 0.2f;

		worldTransformBit[1].translation_.x -= V.z * 0.2f;
		worldTransformBit[1].translation_.y += r;
		worldTransformBit[1].translation_.z += V.x * 0.2f;

		worldTransformBit[2].translation_.x += V.z * 0.4f;
		worldTransformBit[2].translation_.z -= V.x * 0.4f;

		worldTransformBit[3].translation_.x -= V.z * 0.4f;
		worldTransformBit[3].translation_.z += V.x * 0.4f;

		worldTransformBit[4].translation_.x += V.z * 0.2f;
		worldTransformBit[4].translation_.y -= r;
		worldTransformBit[4].translation_.z -= V.x * 0.2f;

		worldTransformBit[5].translation_.x -= V.z * 0.2f;
		worldTransformBit[5].translation_.y -= r;
		worldTransformBit[5].translation_.z += V.x * 0.2f;

		//元
		/*worldTransformBit[0].translation_.y += r * 1.5f;

		worldTransformBit[1].translation_.x += V.z * 0.5f;
		worldTransformBit[1].translation_.y -= r;
		worldTransformBit[1].translation_.z -= V.x * 0.5f;

		worldTransformBit[2].translation_.x -= V.z * 0.5f;
		worldTransformBit[2].translation_.y -= r;
		worldTransformBit[2].translation_.z += V.x * 0.5f;*/

		worldTransformBit[i].rotation_ = worldTransform.rotation_;

		worldTransformBit[i].rotation_.z = PI * rotaSpeed * 1.5f;

		/*worldTransformBit[0].rotation_.z = PI * rotaSpeed * 1.5f;
		worldTransformBit[1].rotation_.z = PI * rotaSpeed * 1.0f;
		worldTransformBit[2].rotation_.z = PI * rotaSpeed * 0.5f;*/

		worldTransformBit[i].matWorld_ = MathUtility::Matrix4Identity();

		worldTransformBit[i].matWorld_ =
			ScaleMatrix4(worldTransformBit[i].matWorld_, worldTransformBit[i].scale_);

		worldTransformBit[i].matWorld_ =
			RotationZMatrix4(worldTransformBit[i].matWorld_, worldTransformBit[i].rotation_);

		worldTransformBit[i].matWorld_ =
			RotationYMatrix4(worldTransformBit[i].matWorld_, worldTransformBit[i].rotation_);

		worldTransformBit[i].matWorld_ =
			MoveMatrix4(worldTransformBit[i].matWorld_, worldTransformBit[i].translation_);

		//行列の再計算
		worldTransformBit[i].TransferMatrix();
	}
}

void PlayerBits::Draw(ViewProjection* viewProjection) {
	// 3Dモデルを描画
	for (int i = 0; i < bitMax; i++)
	{
		model_->Draw(worldTransformBit[i], *viewProjection);
	}
}


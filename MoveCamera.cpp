#include "MoveCamera.h"
#include "Easing.h"

void MoveCamera::Initialize() {

}

void MoveCamera::Appear(ViewProjection* viewProjection, Enemy* enemy) {
	//カメラ初期位置
	viewProjection->eye.x = 10.0f;
	viewProjection->eye.y = 0.0f;
	viewProjection->eye.z = -25.0f;
	viewProjection->target.x = 0.0f;

	if (enemy->GetAppearTimer() < 50.0f) {
		viewProjection->target.y = enemy->GetWorldTransform().matWorld_.m[3][1];
	}
	else if (enemy->GetAppearTimer() >= 50.0f && enemy->GetAppearTimer() < 150.0f) {

		if (flame1 < MAX_FLAME) {
			flame1++;
		}
	}
	else if (enemy->GetAppearTimer() >= 150.0f && enemy->GetAppearTimer() < 300.0f) {
		if (flame2 < MAX_FLAME) {
			flame2++;
		}
	}

	else if (enemy->GetAppearTimer() >= 300.0f) {
		if (flame3 < MAX_FLAME) {
			flame3++;
		}
	}

	//カメラ移動1回目
	viewProjection->eye.z -= easeInOutSine(flame1 / MAX_FLAME) * (viewProjection->eye.z + 100.0f);
	//カメラ移動2回目
	viewProjection->eye.z -= easeInOutSine(flame2 / MAX_FLAME) * (viewProjection->eye.z + 90.0f);
	viewProjection->eye.x -= easeInOutSine(flame2 / MAX_FLAME) * (viewProjection->eye.x + 10.0f);
	viewProjection->target.x += easeInOutSine(flame2 / MAX_FLAME) * (viewProjection->target.x + 10.0f);
	//カメラ移動3回目
	viewProjection->eye.x -= easeInOutSine(flame3 / MAX_FLAME) * (viewProjection->eye.x +  0.0f);
	viewProjection->eye.y += easeInOutSine(flame3 / MAX_FLAME) * (viewProjection->eye.y + 10.0f);
	viewProjection->eye.z -= easeInOutSine(flame3 / MAX_FLAME) * (viewProjection->eye.z + 120.0f);
	viewProjection->target.x -= easeInOutSine(flame3 / MAX_FLAME) * (viewProjection->target.x + 0.0f);
}
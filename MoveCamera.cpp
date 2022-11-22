#include "MoveCamera.h"

void MoveCamera::Initialize()
{
	input_ = Input::GetInstance();

	flame1 = 0.0f;
	flame2 = 0.0f;
	flame3 = 0.0f;
}

//�o�ꎞ�̏���
void MoveCamera::Appear(ViewProjection* viewProjection, Enemy* enemy)
{
	//�J���������ʒu
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

	//�J�����ړ�1���
	viewProjection->eye.z -= easeInOutSine(flame1 / MAX_FLAME) * (viewProjection->eye.z + 100.0f);
	//�J�����ړ�2���
	viewProjection->eye.z -= easeInOutSine(flame2 / MAX_FLAME) * (viewProjection->eye.z + 90.0f);
	viewProjection->eye.x -= easeInOutSine(flame2 / MAX_FLAME) * (viewProjection->eye.x + 10.0f);
	viewProjection->target.x += easeInOutSine(flame2 / MAX_FLAME) * (viewProjection->target.x + 10.0f);
	//�J�����ړ�3���
	viewProjection->eye.x -= easeOutSine(flame3 / MAX_FLAME) * (viewProjection->eye.x + 0.0f);
	viewProjection->eye.y += easeOutSine(flame3 / MAX_FLAME) * (viewProjection->eye.y + 20.0f);
	viewProjection->eye.z -= easeOutSine(flame3 / MAX_FLAME) * (viewProjection->eye.z + 120.0f);
	viewProjection->target.x -= easeOutSine(flame3 / MAX_FLAME) * (viewProjection->target.x + 0.0f);
}

//���j���̏���
void MoveCamera::Defeat(ViewProjection* viewProjection, Vector3 eye, Enemy* enemy)
{
	//�J���������ʒu
	viewProjection->eye = eye;

	if (flame1 < MAX_FLAME) {
		flame1++;
	}

	viewProjection->eye.z -= easeInOutSine(flame1 / MAX_FLAME) * (viewProjection->eye.z + 60.0f);
	viewProjection->eye.y += easeInOutSine(flame1 / MAX_FLAME) * (viewProjection->eye.y - 30.0f);
	viewProjection->eye.x -= easeInOutSine(flame1 / MAX_FLAME) * (viewProjection->eye.x + 0.0f);
}
#include "EnemyShield.h"
#include "Affine.h"

void EnemyShield::Initialize(Model* model) {
	input_ = Input::GetInstance();

	//�ϐ�������
	//�V�[���h��HP
	shieldHP1 = SHIELD_HP_MAX;
	shieldHP2 = SHIELD_HP_MAX;
	distance1 = DISTANCE_MIN * 10.0f;
	distance2 = DISTANCE_MIN * 10.0f;
	revivalTimer = REVIVAL_TIMER_MAX;
	radian = 0.0f;

	for (int i = 0; i < SHIELD_MODEL; i++) {
		//���[���h�ϊ��f�[�^�̏�����
		worldTransform_[i].Initialize();

		//���f��������
		model_[i] = model;

		//�������W�ݒ�
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

		//�A�t�B���ϊ�
		Affine::CreateAffine(worldTransform_[i]);

		//�s��X�V
		worldTransform_[i].TransferMatrix();

	}

}

void EnemyShield::Update() {

	//�L�[���͂�HP�ϓ�
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

	//�V�[���h��HP��0�ɂȂ����狗���𗣂�
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

	//�V�[���h��HP��0�ɂȂ����畜���^�C�}�[�X�^�[�g
	if (shieldHP2 <= 0) {
		revivalTimer--;
	}
	//�^�C�}�[��0�ɂȂ�����V�[���h����
	if (revivalTimer <= 0) {
		shieldHP1 = SHIELD_HP_MAX;
		shieldHP2 = SHIELD_HP_MAX;
		revivalTimer = REVIVAL_TIMER_MAX;
	}

	radian += velocity;
	//���W�A����2�ȏ�Ȃ�0�ɖ߂�
	if (radian >= 2.0f) {
		radian = 0.0f;
	}

	for (int i = 0; i < SHIELD_MODEL; i++) {

		//���ړ�
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

		//HP�ɍ��킹�đ傫���ύX
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

		//�A�t�B���ϊ�
		Affine::CreateAffine(worldTransform_[i]);

		// �s��̓]��
		worldTransform_[i].TransferMatrix();
	}

}

void EnemyShield::Draw(const ViewProjection& viewProjection) {
	for (int i = 0; i < SHIELD_MODEL; i++) {
		model_[i]->Draw(worldTransform_[i], viewProjection);
	}
}
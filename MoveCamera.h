#pragma once
//#include "ViewProjection.h"
#include "Enemy.h"
#include "Input.h"

//�ő�t���[����
static const float MAX_FLAME = 100.0f;

static const float PI = 3.14159f;

class MoveCamera
{
public:
	//������
	void Initialize();
	//�o�ꎞ�̏���
	void Appear(ViewProjection* viewProjection, Enemy* enemy);
	//���j���̏���
	void Defeat(ViewProjection* viewProjection,Vector3 eye, Enemy* enemy);

	//�Q�b�^�[
	float GetFlame3() { return flame3; }

	//�C�[�W���O
	float easeOutSine(float number) {
		return 1 - pow(1 - number, 3);
		//return sin((number * PI) / 2);
	}
	
	float easeInOutSine(float number) {
		return -(cos(PI * number) - 1) / 2;
		//return sin((number * PI) / 2);
	}
private:
	//�L�[����
	Input* input_ = nullptr;

	//�C�[�W���O�̃t���[����
	float flame1;
	float flame2;
	float flame3;

};


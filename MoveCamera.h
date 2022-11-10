#pragma once
//#include "ViewProjection.h"
#include "Enemy.h"

class MoveCamera
{
public:
	void Initialize();

	void Appear(ViewProjection* viewProjection, Enemy* enemy);

private:

	//�C�[�W���O�̃t���[����
	const float MAX_FLAME = 100.0f;
	float flame1 = 0.0f;
	float flame2 = 0.0f;
	float flame3 = 0.0f;

};


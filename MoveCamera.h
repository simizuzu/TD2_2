#pragma once
//#include "ViewProjection.h"
#include "Enemy.h"
#include "Input.h"

//最大フレーム数
static const float MAX_FLAME = 100.0f;

static const float PI = 3.14159f;

class MoveCamera
{
public:
	//初期化
	void Initialize();
	//登場時の処理
	void Appear(ViewProjection* viewProjection, Enemy* enemy);
	//撃破時の処理
	void Defeat(ViewProjection* viewProjection,Vector3 eye, Enemy* enemy);

	//ゲッター
	float GetFlame3() { return flame3; }

	//イージング
	float easeOutSine(float number) {
		return 1 - pow(1 - number, 3);
		//return sin((number * PI) / 2);
	}
	
	float easeInOutSine(float number) {
		return -(cos(PI * number) - 1) / 2;
		//return sin((number * PI) / 2);
	}
private:
	//キー入力
	Input* input_ = nullptr;

	//イージングのフレーム数
	float flame1;
	float flame2;
	float flame3;

};


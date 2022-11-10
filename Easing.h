#pragma once
#include "DirectXCommon.h"

const float PI = 3.14159f;

//イージング
float easeOutSine(float number) {
	return 1 - pow(1 - number, 3);
	//return sin((number * PI) / 2);
}

float easeInOutSine(float number) {
	return -(cos(PI * number) - 1) / 2;
	//return sin((number * PI) / 2);
}

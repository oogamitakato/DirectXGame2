#include "mathMyFunc.h"

//ラジアン変換
float ConversionRadian(float degree) {
	degree = degree * PI / 180;
	return degree;
}

//度変換
float ConversionDegree(float radian) {
	radian = radian * 180 / PI;
	return radian;
}
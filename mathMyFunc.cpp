#include "mathMyFunc.h"

//���W�A���ϊ�
float ConversionRadian(float degree) {
	degree = degree * PI / 180;
	return degree;
}

//�x�ϊ�
float ConversionDegree(float radian) {
	radian = radian * 180 / PI;
	return radian;
}
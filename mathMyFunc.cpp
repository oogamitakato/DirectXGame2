#include "mathMyFunc.h"

//ƒ‰ƒWƒAƒ“•ÏŠ·
float ConversionRadian(float degree) {
	degree = degree * PI / 180;
	return degree;
}

//“x•ÏŠ·
float ConversionDegree(float radian) {
	radian = radian * 180 / PI;
	return radian;
}
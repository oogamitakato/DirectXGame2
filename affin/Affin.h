#pragma once
#include "Model.h"
#include "WorldTransform.h"

//スケーリング設定
void ConversionScale(float& x, float& y, float& z, WorldTransform& worldTransform);

//回転角設定
void ConversionRot(float &x, float &y, float &z, WorldTransform &worldTransform);

//平行移動設定
void ConversionTrans(float x, float y, float z,WorldTransform &worldTransform);
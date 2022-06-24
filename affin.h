#pragma once
#include "Model.h"
#include "WorldTransform.h"

struct Affin {
  private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;

  public:
	
	//スケーリング設定
	void ConversionScale(float& x, float& y, float& z);

	//回転角設定
	void ConversionRot(float& x, float& y, float& z);

	//平行移動設定
	void ConversionTrans(float& x, float& y, float& z);

};

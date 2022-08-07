#pragma once
#include "Affin.h"
#include "DebugText.h"
#include "Model.h"
#include "WorldTransform.h"

class Field 
{
  private:
	//ワールド変換データ
	WorldTransform worldTransform_[20];
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Vector3 scale = {5.0f, 5.0f, 5.0f};

  public:
	//初期化
	Field(Model* model, const Vector3 position);

	//更新
	void Update();

	//描画
	void Draw(const ViewProjection& viewProjection);
};

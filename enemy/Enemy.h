#pragma once
#include "Affin.h"
#include "DebugText.h"
#include "Model.h"
#include "WorldTransform.h"

class Enemy {
  private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//速度
	Vector3 velocity_ = {0.0f,0.0f,-0.1f};

  public:
	 //初期化
	Enemy(Model* model);

	//更新
	void Update();

	//描画
	void Draw(const ViewProjection& viewProjection);

};
#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Affin.h"

//自キャラの弾
class PlayerBullet {
  public:
	  //初期化
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	//更新
	void Update();

	//描画
	void Draw(const ViewProjection& viewProjection);

  private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//速度
	Vector3 velocity_;
};

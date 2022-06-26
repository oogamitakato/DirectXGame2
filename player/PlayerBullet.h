#pragma once
#include "Affin.h"
#include "DebugText.h"
#include "Model.h"
#include "WorldTransform.h"

//自キャラの弾
class PlayerBullet {
  private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//速度
	Vector3 velocity_;

	//弾の寿命
	static const int32_t kLifeTimer = 60 * 5;
	//デスタイマー
	int32_t debugTimer_ = kLifeTimer;
	//デスフラグ
	bool isDead_ = false;

  public:
	//初期化
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	//更新
	void Update();

	//描画
	void Draw(const ViewProjection& viewProjection);

	//ゲッター
	bool IsDead() const { return isDead_; }
};

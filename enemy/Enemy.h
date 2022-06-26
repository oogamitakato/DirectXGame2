#pragma once
#include "Affin.h"
#include "DebugText.h"
#include "Model.h"
#include "WorldTransform.h"

class Enemy {
  private:
	//行動フェーズ
	enum class Phase {
		Approach, //接近する
		Leave,    //離脱する
	};

	//フェーズ
	Phase phase_ = Phase::Approach;

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//速度
	Vector3 approachVelocity_ = {0.0f, 0.0f, -0.1f};
	Vector3 leaveVelocity_ = {0.1f, 0.1f, 0.0f};

  public:
	//初期化
	Enemy(Model* model);

	//更新
	void Update();

	//描画
	void Draw(const ViewProjection& viewProjection);

	/*フェーズごとの関数*/
	//接近フェーズ
	void Approach();
	//離脱フェーズ
	void Leave();
};
#pragma once
#include "Affin.h"
#include "DebugText.h"
#include "Model.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"
#include "Player.h"

//自機クラスの前方処理
class Player;

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

	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//発射タイマー
	int32_t fireTimer = 0;

  public:
	//初期化
	Enemy(Model* model);

	//弾発射
	void Fire();

	//更新
	void Update();

	//描画
	void Draw(const ViewProjection& viewProjection);

	/*フェーズごとの関数*/
	//接近フェーズ初期化
	void InitApproach();
	//接近フェーズ
	void Approach();

	//離脱フェーズ初期化
	void InitLeave();
	//離脱フェーズ
	void Leave();

	//発射間隔
	static const int kFireInterval = 60;

	// setter
	void SetPlayer(Player* player) { player_ = player; }

	//自キャラ
	Player* player_ = nullptr;

	//ワールド座標を取得
	Vector3 GetWorldPosition();
};
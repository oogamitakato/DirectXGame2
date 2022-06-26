#include "Enemy.h"
#include <cassert>

//初期化(コンストラクタ)
Enemy::Enemy(Model* model) {
	// NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = TextureManager::Load("uvChecker.png");
	//ワールド変換データの初期化
	worldTransform_.Initialize();

	//初期座標
	worldTransform_.translation_ = {0.0f, 2.0f, 20.0f};
}

//更新
void Enemy::Update() {

	switch (phase_) {
	case Phase::Approach:
	default:
		Approach();
		break;

	case Phase::Leave:
		Leave();
		break;
	}


	worldTransform_.matWorld_.m[3][0] = worldTransform_.translation_.x;
	worldTransform_.matWorld_.m[3][1] = worldTransform_.translation_.y;
	worldTransform_.matWorld_.m[3][2] = worldTransform_.translation_.z;

	//行列更新
	worldTransform_.TransferMatrix();
}

//描画
void Enemy::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

/*フェーズごとの関数*/
//接近フェーズ
void Enemy::Approach() {
	//座標を移動させる
	worldTransform_.translation_ += approachVelocity_;
	//規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}
//離脱フェーズ
void Enemy::Leave() {
	//移動
	worldTransform_.translation_ += leaveVelocity_;
}
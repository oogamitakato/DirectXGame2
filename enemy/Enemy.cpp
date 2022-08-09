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
	worldTransform_.translation_ = {-10.0f, -10.0f, 20.0f};

	//接近フェーズ初期化
	InitApproach();
}

//敵の弾発射
void Enemy::Fire() {
	//弾の速度
	const float kBulletSpeed = -0.5f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();

	newBullet->Initialize(
	  model_,
	  {
	    worldTransform_.translation_.x,
	    worldTransform_.translation_.y,
	    worldTransform_.translation_.z,
	  },
	  velocity);

	//弾を登録する
	bullets_.push_back(std::move(newBullet));
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

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });


	/*ConversionTrans(
	  approachVelocity_.x, approachVelocity_.y, approachVelocity_.z, worldTransform_);*/
	worldTransform_.matWorld_.m[3][0] = worldTransform_.translation_.x;
	worldTransform_.matWorld_.m[3][1] = worldTransform_.translation_.y;
	worldTransform_.matWorld_.m[3][2] = worldTransform_.translation_.z;

	//行列更新
	worldTransform_.TransferMatrix();

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}
}

//描画
void Enemy::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

/*フェーズごとの関数*/
//接近フェーズ初期化
void Enemy::InitApproach() {
	//発射タイマーを初期化
	//fireTimer = kFireInterval;
}

//接近フェーズ
void Enemy::Approach() {
	//座標を移動させる
	worldTransform_.translation_ += approachVelocity_;
	//規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}

	////発射タイマーカウントダウン
	//fireTimer--;

	////指定時間に達した
	//if (fireTimer <= 0) {
	//	//弾発射
	//	Fire();
	//	//発射タイマーを初期化
	//	fireTimer = kFireInterval;
	//}
}

//接近フェーズ初期化
void Enemy::InitLeave() {
	//発射タイマーを初期化
	fireTimer = kFireInterval;
}

//離脱フェーズ
void Enemy::Leave() {
	//移動
	worldTransform_.translation_ += leaveVelocity_;

	//発射タイマーカウントダウン
	fireTimer--;

	//指定時間に達した
	if (fireTimer <= 0) {
		//弾発射
		Fire();
		//発射タイマーを初期化
		fireTimer = kFireInterval;
	}
}
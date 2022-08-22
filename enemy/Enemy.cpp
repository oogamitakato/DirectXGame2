#include "Enemy.h"
#include <cassert>

//初期化(コンストラクタ)
Enemy::Enemy(Model* model) {
	// NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = TextureManager::Load("3Denemy.png");
	//ワールド変換データの初期化
	worldTransform_.Initialize();

	//初期座標
	worldTransform_.translation_ = {0.0f, 5.0f, 50.0f};
	worldTransform_.scale_ = {5.0f, 5.0f, 5.0f};

	//接近フェーズ初期化
	InitApproach();
}

//敵の弾発射
void Enemy::Fire() {
	//弾の速度
	const float kBulletSpeed = 0.5f;
	//自キャラのワールド座標を取得
	Vector3 velocity = player_->GetWorldPosition();
	//敵キャラのワールド座標を取得
	Vector3 enemyPos = GetWorldPosition();
	//敵キャラ→自キャラの差分ベクトルを求める
	velocity -= enemyPos;
	//べクトルの正規化
	velocity /= sqrt(pow(velocity.x, 2.0f) + pow(velocity.y, 2.0f) + pow(velocity.z, 2.0f));
	//ベクトルの長さを、速さに合わせる
	velocity *= kBulletSpeed;

	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

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


	worldTransform_.matWorld_.m[0][0] = worldTransform_.scale_.x;
	worldTransform_.matWorld_.m[1][1] = worldTransform_.scale_.y;
	worldTransform_.matWorld_.m[2][2] = worldTransform_.scale_.z;

	worldTransform_.matWorld_.m[3][0] = worldTransform_.translation_.x;
	worldTransform_.matWorld_.m[3][1] = worldTransform_.translation_.y;
	worldTransform_.matWorld_.m[3][2] = worldTransform_.translation_.z;

	//行列更新
	worldTransform_.TransferMatrix();

	//弾更新
	if (isDead_ == false) {
		for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
			bullet->Update();
		}
	}
}

//描画
void Enemy::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	if (isDead_ == false) {
		model_->Draw(worldTransform_, viewProjection, textureHandle_);

		for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
			bullet->Draw(viewProjection);
		}
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
	if (worldTransform_.translation_.z < 20.0f) {
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
	if (worldTransform_.translation_.x > 30) {
		leaveVelocity_ = -leaveVelocity_;
	} else if (worldTransform_.translation_.x < -30) {
		leaveVelocity_ = -leaveVelocity_;
	}
	//規定の位置に到達したら離脱
	if (worldTransform_.translation_.z > 60.0f) {
		phase_ = Phase::Approach;
	}

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

//ワールド座標を取得
Vector3 Enemy::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

//衝突を検出したら呼び出される関数
void Enemy::OnCollision() { 

	hp--;
	if (hp <= 0) {
		isDead_ = true;	
	}
}
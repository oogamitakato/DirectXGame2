#include "EnemyBullet.h"
#include <cassert>

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);

	model_ = model;

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("tex1.png");

	//ワールド変換データの初期化
	worldTransform_.Initialize();

	//スケーリング設定
	ConversionScale(0.5f, 0.5f, 0.5f, worldTransform_);

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = {position.x, position.y, position.z};

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}

void EnemyBullet::Update(){
	//座標を移動させる
	worldTransform_.translation_ += velocity_;

	worldTransform_.matWorld_.m[3][0] = worldTransform_.translation_.x;
	worldTransform_.matWorld_.m[3][1] = worldTransform_.translation_.y;
	worldTransform_.matWorld_.m[3][2] = worldTransform_.translation_.z;

	//時間経過でデス
	if (--debugTimer_ <= 0) {
		isDead_ = true;
	}

	//行列更新
	worldTransform_.TransferMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

//衝突を検出したら呼び出されるコールバック関数
void EnemyBullet::OnCollision() { isDead_ = true; }

//ワールド座標を取得
Vector3 EnemyBullet::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
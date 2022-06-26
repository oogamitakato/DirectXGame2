#include "Enemy.h"
#include <cassert>

//初期化(コンストラクタ)
void Enemy::Initialize(Model* model) {
	// NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = TextureManager::Load("uvChecker.png");

	//ワールド変換データの初期化
	worldTransform_.Initialize();

	worldTransform_.matWorld_.m[3][2] = 10;
}

//更新
void Enemy::Update() { 
	//座標を移動させる
	worldTransform_.translation_ += velocity_;

	worldTransform_.matWorld_.m[3][0] = worldTransform_.translation_.x;
	worldTransform_.matWorld_.m[3][1] = worldTransform_.translation_.y;
	worldTransform_.matWorld_.m[3][2] = worldTransform_.translation_.z;
}

//描画
void Enemy::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
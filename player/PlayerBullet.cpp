#include "PlayerBullet.h"
#include "Affin.h"
#include "Player.h"
#include <cassert>

//初期化
void PlayerBullet::Initialize(Model* model, const Vector3& position) {
	// NULLポインタチェック
	assert(model);

	model_ = model;

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("tex1.png");

	debugText_ = DebugText::GetInstance();

	//ワールド変換データの初期化
	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = {position.x, position.y, position.z};
}

//更新
void PlayerBullet::Update() {

	worldTransform_.matWorld_.m[3][0] = worldTransform_.translation_.x;
	worldTransform_.matWorld_.m[3][1] = worldTransform_.translation_.y;
	worldTransform_.matWorld_.m[3][2] = worldTransform_.translation_.z;

	//行列更新
	worldTransform_.TransferMatrix();
}

//描画
void PlayerBullet::Draw(const ViewProjection& viewProjection) {

	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
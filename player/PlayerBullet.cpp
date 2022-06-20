#include "PlayerBullet.h"
#include "Player.h"
#include <cassert>

//初期化
void PlayerBullet::Initialize(Model* model, const Vector3& position) {
	//NULLポインタチェック
	assert(model);

	model_ = model;

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("tex1.png");

	//ワールド変換データの初期化
	worldTransform_.Initialize();

	//引数で受け取った初期座標をリセット
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
}

//更新
void PlayerBullet::Update() { 


	//行列更新
	worldTransform_.TransferMatrix();
}

//描画
void PlayerBullet::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
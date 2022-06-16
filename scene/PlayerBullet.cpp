#include "PlayerBullet.h"
#include <cassert>

//初期化
void PlayerBullet::Initialize(Model* model, const Vector3& position) {
	//NULLポインタチェック
	assert(model);

	model_ = model;

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("tex1.png");


}

//更新
void PlayerBullet::Update();

//描画
void PlayerBullet::Draw(const ViewProjection& viewProjection);
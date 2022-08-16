#include "skydome.h"
#include "DebugText.h"
#include "Input.h"
#include <cassert>
#include <math.h>

void Skydome::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;

	//ワールド変換データの初期化
	worldTransform_.Initialize();

	worldTransform_.matWorld_.m[0][0] = 1000.0f;
	worldTransform_.matWorld_.m[1][1] = 1000.0f;
	worldTransform_.matWorld_.m[2][2] = 1000.0f;

		//行列更新
	worldTransform_.TransferMatrix();
}

void Skydome::Update() {

}

void Skydome::Draw(ViewProjection viewProjection) {
	//3Dモデル描画
	model_->Draw(worldTransform_, viewProjection);
}
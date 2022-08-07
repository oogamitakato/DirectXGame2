#include "Field.h"
#include "Affin.h"
#include <memory>
#include <list>
#include <cassert>

Field::Field(Model* model) {
	// NULLポインタチェック
	assert(model);

	model_ = model;

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("mario.jpg");

	for (int i = 0; i < 20; i++) {
		//ワールド変換データの初期化
		worldTransform_[i].Initialize();

		//引数で受け取った初期座標をセット
		ConversionTrans(-10 + i, 4 * pow(-1, i), 0, worldTransform_[i]);
		ConversionScale(scale.x, scale.y, scale.z, worldTransform_[i]);

		worldTransform_[i].TransferMatrix();
	}
}

void Field::Update() {

}

void Field::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	for (int i = 0; i < 20; i++) {
		model_->Draw(worldTransform_[i], viewProjection, textureHandle_);
	}
}
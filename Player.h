#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "DebugText.h"

class Player {
  private:
	/*メンバ変数*/
	const float PI = 3.1415;
	float speed = 0.5f;

	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Input* input_;
	DebugText* debugText_;

  public:
	/*メンバ関数*/

	//初期化(コンストラクタ)
	Player(Model* model, uint32_t textureHandle);

	//回転
	void Rotate();

	//更新
	void Update();

	//描画
	void Draw(ViewProjection viewProjection);
};
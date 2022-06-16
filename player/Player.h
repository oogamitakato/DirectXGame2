#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "DebugText.h"
#include "player//PlayerBullet.h"

class Player {
  private:
	/*メンバ変数*/
	const float PI = 3.1415;
	float speed = 0.5f;
	Vector3 move;

	Matrix4 matTrans = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, move.x, move.y, move.z, 1};

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Input* input_;
	DebugText* debugText_;

	//弾
	PlayerBullet* bullet_ = nullptr;

  public:
	/*メンバ関数*/

	//初期化(コンストラクタ)
	Player(Model* model, uint32_t textureHandle);

	////スケーリング設定
	//void ConversionScale(float& x, float& y, float& z);

	////回転角設定
	//void ConversionRot(float& x, float& y, float& z);

	////平行移動設定
	//void ConversionTrans(float& x, float& y, float& z);

	//更新
	void Update();

	//描画
	void Draw(ViewProjection viewProjection);

	//攻撃
	void Attack();
};
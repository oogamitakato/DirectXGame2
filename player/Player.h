#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "DebugText.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>

class Player {
  private:
	/*メンバ変数*/
	const float PI = 3.1415;
	float speed = 0.5f;
	Vector3 move;

	Matrix4 matTrans;

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Input* input_;
	DebugText* debugText_;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	Matrix4 matRot;

  public:
	/*メンバ関数*/

	//初期化(コンストラクタ)
	Player(Model* model, uint32_t textureHandle);

	//更新
	void Update();

	//描画
	void Draw(ViewProjection viewProjection);

	//攻撃
	void Attack();

	//回転
	void Rotate();
	//弾の発射方向の回転
	Vector3 AttackRotate(Vector3 rotation, WorldTransform worldTransform);

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//衝突を検出したら呼び出される関数
	void OnCollision();

	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }
};
#include "Player.h"
#include "Affin.h"
#include "DebugText.h"
#include "Input.h"
#include <cassert>
#include <math.h>

//初期化(コンストラクタ)
Player::Player(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換データの初期化
	worldTransform_.Initialize();
	worldTransform_.matWorld_.m[3][0] = 5;
}

//更新
void Player::Update() {

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { 
		return bullet->IsDead();
		});

	Rotate();

	move = {0, 0, 0};

	//移動限界座標
	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 20.0f;

	//範囲を超えない処理
	worldTransform_.matWorld_.m[3][0] = max(worldTransform_.matWorld_.m[3][0], -kMoveLimitX);
	worldTransform_.matWorld_.m[3][0] = min(worldTransform_.matWorld_.m[3][0], +kMoveLimitX);
	worldTransform_.matWorld_.m[3][1] = max(worldTransform_.matWorld_.m[3][1], -kMoveLimitY);
	worldTransform_.matWorld_.m[3][1] = min(worldTransform_.matWorld_.m[3][1], +kMoveLimitY);

	//キーボード入力による移動処理
	if (input_->PushKey(DIK_UP)) {
		move.y += speed;
	}

	if (input_->PushKey(DIK_DOWN)) {
		move.y -= speed;
	}

	if (input_->PushKey(DIK_RIGHT)) {
		move.x += speed;
	}

	if (input_->PushKey(DIK_LEFT)) {
		move.x -= speed;
	}

	//座標移動
	matTrans = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, move.x, move.y, move.z, 1};

	worldTransform_.matWorld_ *= matTrans;

	//キャラクター攻撃処理
	Attack();

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//行列更新
	worldTransform_.TransferMatrix();

	//キャラクターの座標を画面表示する処理
	debugText_->SetPos(50, 100);
	debugText_->Printf(
	  "Player : {%f,%f,%f}", worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1],
	  worldTransform_.matWorld_.m[3][2]);
}

//描画
void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

//攻撃
void Player::Attack() {

	if (input_->TriggerKey(DIK_SPACE)) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = AttackRotate(velocity, worldTransform_);

		//弾を生成し、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(
		  model_,
		  {
		    worldTransform_.matWorld_.m[3][0],
		    worldTransform_.matWorld_.m[3][1],
		    worldTransform_.matWorld_.m[3][2],
		  },
		  velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));
	}
}

//回転
void Player::Rotate() {

	//回転行列
	matRot = {
	  cos(worldTransform_.rotation_.y),
	  0.0f,
	  -sin(worldTransform_.rotation_.y),
	  0.0f,
	  0.0f,
	  1.0f,
	  0.0f,
	  0.0f,
	  sin(worldTransform_.rotation_.y),
	  0.0f,
	  cos(worldTransform_.rotation_.y),
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  1.0f};

	//現在の平行移動座標を保持
	Vector3 move2 = {
	  worldTransform_.matWorld_.m[3][0],
	  worldTransform_.matWorld_.m[3][1],
	  worldTransform_.matWorld_.m[3][2],
	};
	//座標を０にする
	worldTransform_.matWorld_.m[3][0] = 0;
	worldTransform_.matWorld_.m[3][1] = 0;
	worldTransform_.matWorld_.m[3][2] = 0;

	//キーを押して回転
	if (input_->PushKey(DIK_Q)) {
		worldTransform_.rotation_.y = 0.05f;
	} else if (input_->PushKey(DIK_E)) {
		worldTransform_.rotation_.y = -0.05f;
	} else {
		worldTransform_.rotation_.y = 0.0f;
	}

	//行列を掛け算
	worldTransform_.matWorld_ *= matRot;
	//座標をもとの位置に戻す
	worldTransform_.matWorld_.m[3][0] = move2.x;
	worldTransform_.matWorld_.m[3][1] = move2.y;
	worldTransform_.matWorld_.m[3][2] = move2.z;
}

//弾の発射方向の回転
Vector3 Player::AttackRotate(Vector3 rotation, WorldTransform worldTransform) {

	rotation = {
	  rotation.x * worldTransform_.matWorld_.m[0][0] +
	    rotation.y * worldTransform_.matWorld_.m[1][0] +
	    rotation.z * worldTransform_.matWorld_.m[2][0],
	  rotation.x * worldTransform_.matWorld_.m[0][1] +
	    rotation.y * worldTransform_.matWorld_.m[1][1] +
	    rotation.z * worldTransform_.matWorld_.m[2][1],
	  rotation.x * worldTransform_.matWorld_.m[0][2] +
	    rotation.y * worldTransform_.matWorld_.m[1][2] +
	    rotation.z * worldTransform_.matWorld_.m[2][2],
	};

	return rotation;
}

#include "Player.h"
#include "Input.h"
#include "DebugText.h"
#include <math.h>
#include <cassert>

//初期化(コンストラクタ)
Player::Player(Model* model, uint32_t textureHandle) 
{
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
}

//更新
void Player::Update() { 
	Move();
	Rotate();

	//キャラクター攻撃処理
	Attack();

	//弾更新
	if (bullet_) {
		bullet_->Update();
	}
}

//移動
void Player::Move() {
	Vector3 move = {0, 0, 0};

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
	Matrix4 matTrans(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, move.x, move.y, move.z, 1);

	worldTransform_.matWorld_ *= matTrans;

	//行列更新
	worldTransform_.TransferMatrix();

	//キャラクターの座標を画面表示する処理
	debugText_->SetPos(50, 100);
	debugText_->Printf(
	  "Player : {%f,%f,%f}", worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1],
	  worldTransform_.matWorld_.m[3][2]);
}

//回転
void Player::Rotate() { 
	
	Vector3 rotate = {0.0f, 0.0f, 0.0f};

	if (input_->PushKey(DIK_Q)) {
		rotate.y += 0.05;
	}

	if (input_->PushKey(DIK_E)) {
		rotate.y -= 0.05;
	}

	//座標移動
	Matrix4 matRotY(
	  cos(rotate.y), 0.0f, sin(rotate.y), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -sin(rotate.y), 0.0f,
	  cos(rotate.y), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	worldTransform_.matWorld_ *= matRotY;

	//行列更新
	worldTransform_.TransferMatrix();
}

//攻撃
void Player::Attack() { 
	
	if (input_->PushKey(DIK_SPACE)) {
		//弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(
		  model_, {worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1],
		           worldTransform_.matWorld_.m[3][2]});

		//弾を登録する
		bullet_ = newBullet;
	}
};

//描画
void Player::Draw(ViewProjection viewProjection){

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾描画
	if (bullet_) {
		bullet_->Draw(viewProjection);
	}
}


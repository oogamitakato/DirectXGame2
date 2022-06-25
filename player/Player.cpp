#include "Player.h"
#include "Affin.h"
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
	worldTransform_.matWorld_.m[3][1] = 5;
}

//更新
void Player::Update() 
{ 
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
	if (input_->PushKey(DIK_UP)) 
	{
		move.y += speed;
	}

	if (input_->PushKey(DIK_DOWN)) 
	{
		move.y -= speed;
	}

	if (input_->PushKey(DIK_RIGHT)) 
	{
		move.x += speed;
	}

	if (input_->PushKey(DIK_LEFT)) 
	{
		move.x -= speed;
	}

	//座標移動	

	Matrix4 matTrans(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		move.x, move.y, move.z, 1);
	
	worldTransform_.matWorld_ *= matTrans;

	//キャラクター攻撃処理
	Attack();

	//弾更新
	if (bullet_) {
		if (input_->PushKey(DIK_SPACE)) {
			bullet_->Update();
		}
	}

	//行列更新
	worldTransform_.TransferMatrix();

	//キャラクターの座標を画面表示する処理
	debugText_->SetPos(50, 100);
	debugText_->Printf(
	  "Player : {%f,%f,%f}", 
		worldTransform_.matWorld_.m[3][0],
		worldTransform_.matWorld_.m[3][1],
		worldTransform_.matWorld_.m[3][2]);
}

//描画
void Player::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	if (bullet_) {
		bullet_->Draw(viewProjection);
	}
}

//攻撃
void Player::Attack() {

	if (input_->PushKey(DIK_SPACE)) {
		//弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(
		  model_, {
		            worldTransform_.matWorld_.m[3][0],
		            worldTransform_.matWorld_.m[3][1],
		            worldTransform_.matWorld_.m[3][2],
		          });

		//弾を登録する
		bullet_ = newBullet;
	}
}

//void Player::ConversionScale(float& x, float& y, float& z) {
//
//	// X,Y,Z方向のスケーリングを設定
//	worldTransform_.scale_ = {x, y, z};
//	//スケーリング行列を宣言
//	Matrix4 matScale;
//
//	//スケーリング倍率を行列に設定
//	matScale = {
//	  worldTransform_.scale_.x,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  worldTransform_.scale_.y,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  worldTransform_.scale_.z,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  1.0f};
//
//	// matScaleを掛け算して代入
//	worldTransform_.matWorld_ *= matScale;
//}
//
//void Player::ConversionRot(float& x, float& y, float& z) {
//
//	// X,Y,Z軸周りの回転角を設定
//	worldTransform_.rotation_ = {x, y, z};
//	//合成用回転行列を宣言
//	Matrix4 matRot;
//	//各軸回転行列を宣言
//	Matrix4 matRotX, matRotY, matRotZ;
//
//	// Z軸回転行列を宣言
//	matRotZ = {
//	  cos(worldTransform_.rotation_.z),
//	  sin(worldTransform_.rotation_.z),
//	  0.0f,
//	  0.0f,
//	  -sin(worldTransform_.rotation_.z),
//	  cos(worldTransform_.rotation_.z),
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  1.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  1.0f};
//
//	// X軸回転行列を宣言
//	matRotX = {
//	  1.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  cos(worldTransform_.rotation_.x),
//	  sin(worldTransform_.rotation_.x),
//	  0.0f,
//	  0.0f,
//	  -sin(worldTransform_.rotation_.x),
//	  cos(worldTransform_.rotation_.x),
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  1.0f};
//
//	// Y軸回転行列を宣言
//	matRotY = {
//	  cos(worldTransform_.rotation_.y),
//	  0.0f,
//	  -sin(worldTransform_.rotation_.y),
//	  0.0f,
//	  0.0f,
//	  1.0f,
//	  0.0f,
//	  0.0f,
//	  sin(worldTransform_.rotation_.y),
//	  0.0f,
//	  cos(worldTransform_.rotation_.y),
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  1.0f};
//
//	//各軸の回転行列を合成
//	matRotZ *= matRotX;
//	matRotZ *= matRotY;
//	matRot = matRotZ;
//
//	// matRotを掛け算して代入
//	worldTransform_.matWorld_ *= matRot;
//}
//
//void Player::ConversionTrans(float& x, float& y, float& z) {
//
//	// X,Y,Z軸周りの平行移動を設定
//	worldTransform_.translation_ = {x, y, z};
//	//平行移動行列を宣言
//	Matrix4 matTrans = MathUtility::Matrix4Identity();
//
//	//移動量を行列に設定する
//	matTrans = {
//	  1.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  1.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  0.0f,
//	  1.0f,
//	  0.0f,
//	  worldTransform_.translation_.x,
//	  worldTransform_.translation_.y,
//	  worldTransform_.translation_.z,
//	  1.0f};
//
//	// matTransを掛け算して代入
//	worldTransform_.matWorld_ *= matTrans;
//}
#include "Affin.h"

Affin::Affin(Model* model_) {

}

//スケーリング設定
void Affin::ConversionScale(float& x, float& y, float& z) {
	// X,Y,Z方向のスケーリングを設定
	worldTransform_.scale_ = {x, y, z};
	//スケーリング行列を宣言
	Matrix4 matScale;

	//スケーリング倍率を行列に設定
	matScale = {
	  worldTransform_.scale_.x,
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  worldTransform_.scale_.y,
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  worldTransform_.scale_.z,
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  1.0f};

	// matScaleを掛け算して代入
	worldTransform_.matWorld_ *= matScale;
}

//回転角設定
void Affin::ConversionRot(float& x, float& y, float& z) {
	// X,Y,Z軸周りの回転角を設定
	worldTransform_.rotation_ = {x, y, z};
	//合成用回転行列を宣言
	Matrix4 matRot;
	//各軸回転行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;

	// Z軸回転行列を宣言
	matRotZ = {
	  cos(worldTransform_.rotation_.z),
	  sin(worldTransform_.rotation_.z),
	  0.0f,
	  0.0f,
	  -sin(worldTransform_.rotation_.z),
	  cos(worldTransform_.rotation_.z),
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  1.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  1.0f};

	// X軸回転行列を宣言
	matRotX = {
	  1.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  cos(worldTransform_.rotation_.x),
	  sin(worldTransform_.rotation_.x),
	  0.0f,
	  0.0f,
	  -sin(worldTransform_.rotation_.x),
	  cos(worldTransform_.rotation_.x),
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  1.0f};

	// Y軸回転行列を宣言
	matRotY = {
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

	//各軸の回転行列を合成
	matRotZ *= matRotX;
	matRotZ *= matRotY;
	matRot = matRotZ;

	// matRotを掛け算して代入
	worldTransform_.matWorld_ *= matRot;
}

//平行移動設定
void Affin::ConversionTrans(float& x, float& y, float& z) {
	// X,Y,Z軸周りの平行移動を設定
	worldTransform_.translation_ = {x, y, z};
	//平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	//移動量を行列に設定する
	matTrans = {
	  1.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  1.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  1.0f,
	  0.0f,
	  worldTransform_.translation_.x,
	  worldTransform_.translation_.y,
	  worldTransform_.translation_.z,
	  1.0f};

	// matTransを掛け算して代入
	worldTransform_.matWorld_ *= matTrans;
}

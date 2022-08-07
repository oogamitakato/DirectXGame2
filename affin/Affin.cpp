#include "Affin.h"

//スケーリング設定
void ConversionScale(float& x, float& y, float& z, WorldTransform worldTransform) {
	// X,Y,Z方向のスケーリングを設定
	worldTransform.scale_ = {x, y, z};

	//スケーリング行列を宣言
	Matrix4 matScale;

	//スケーリング倍率を行列に設定
	matScale = {
	  worldTransform.scale_.x,
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  worldTransform.scale_.y,
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  worldTransform.scale_.z,
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  1.0f};

	// matScaleを掛け算して代入
	worldTransform.matWorld_ *= matScale;
}

//回転角設定
void ConversionRot(float x, float y, float z, WorldTransform worldTransform) {
	// X,Y,Z軸周りの回転角を設定
	Vector3 rotation = {x, y, z};

	//合成用回転行列を宣言
	Matrix4 matRot;
	//各軸回転行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;

	// Z軸回転行列を宣言
	matRotZ = {
	  cos(rotation.z),
	  sin(rotation.z),
	  0.0f,
	  0.0f,
	  -sin(rotation.z),
	  cos(rotation.z),
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
	  cos(rotation.x),
	  sin(rotation.x),
	  0.0f,
	  0.0f,
	  -sin(rotation.x),
	  cos(rotation.x),
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  1.0f};

	// Y軸回転行列を宣言
	matRotY = {
	  cos(rotation.y),
	  0.0f,
	  -sin(rotation.y),
	  0.0f,
	  0.0f,
	  1.0f,
	  0.0f,
	  0.0f,
	  sin(rotation.y),
	  0.0f,
	  cos(rotation.y),
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
	worldTransform.matWorld_ *= matRot;
}

//平行移動設定
void ConversionTrans(float x, float y, float z, WorldTransform worldTransform) {
	// X,Y,Z軸周りの平行移動を設定
	Vector3 translation = {x, y, z};

	//平行移動行列を宣言
	Matrix4 matTrans;

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
	  translation.x,
	  translation.y,
	  translation.z,
	  1.0f};

	// matTransを掛け算して代入
	worldTransform.matWorld_ *= matTrans;
}

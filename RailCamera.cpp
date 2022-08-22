#include "RailCamera.h"

void RailCamera::Initialize(Vector3 translation, Vector3 rotation) {
	debugText_ = DebugText::GetInstance();

	//ワールドトランスフォームの初期設定
	worldTransform_.Initialize();
	worldTransform_.translation_ = translation;
	worldTransform_.rotation_ = rotation;

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void RailCamera::Update() {
	worldTransform_.translation_ -= {0.0f, 0.0f, 0.1f};
	worldTransform_.rotation_ += {0.0f, 0.0f, 0.1f};
	worldTransform_.TransferMatrix();
	viewProjection_.eye = worldTransform_.translation_;

	//ワールド前方ベクトル
	Vector3 forward(0, 0, 1);
	//レールカメラの回転を反映
	forward = worldTransform_.rotation_;
	//視点から前方に適当な距離進んだ位置が注視点
	viewProjection_.target.x = viewProjection_.eye.x + forward.x;
	viewProjection_.target.y = viewProjection_.eye.y + forward.y;
	viewProjection_.target.z = viewProjection_.eye.z + forward.z;
	//ワールド上方向ベクトル
	Vector3 up(0, 1, 0);
	//レールカメラの回転を反映(レールカメラの上方向ベクトル)
	viewProjection_.up = worldTransform_.rotation_;
	//ビュープロジェクションを更新
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();

	debugText_->SetPos(50, 120);
	debugText_->Printf(
	  "eye : {%f, %f, %f}", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
}

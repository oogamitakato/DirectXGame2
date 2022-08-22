#pragma once
#include "DebugText.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class RailCamera {
  private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	DebugText* debugText_;

  public:

	//初期化
	void Initialize(Vector3 translation, Vector3 rotation);

	//更新
	void Update();
};

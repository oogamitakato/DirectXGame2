#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include <math.h>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//円周率
	const float PI = 3.14f;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//3Dモデル
	Model* model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//立方体の頂点
	Vector3 vertex[8] = 
	{
	  {-5.0f, -5.0f, -5.0f},
	  {5.0f, -5.0f, -5.0f},
	  {5.0f, 5.0f, -5.0f},
	  {-5.0f, 5.0f, -5.0f},
	  {-5.0f, -5.0f, 5.0f},
	  {5.0f, -5.0f, 5.0f},
	  {5.0f, 5.0f, 5.0f},
	  {-5.0f, 5.0f, 5.0f},
	};

	//立方体の辺
	int eageList[12][2] = 
	{
	  {0,1},
	  {1,2},
	  {2,3},
	  {3,0},
	  {0,4},
	  {1,5},
	  {2,6},
	  {3,7},
	  {4,5},
	  {5,6},
	  {6,7},
	  {7,4},
	};

	//平行移動する立方体の頂点
	Vector3 boxMoved[8];
	Vector3 boxMoved2[8];

	//回転する立方体の頂点
	Vector3 boxRotated[8];
	Vector3 boxRotated2[8];

	//拡大する立方体の頂点
	Vector3 boxScaled[8];
	Vector3 boxScaled2[8];

	//平行移動
	float position[4][4] = 
	{
		{1.0f,0.0f,0.0f,0.0f},
		{0.0f,1.0f,0.0f,0.0f},
		{0.0f,0.0f,1.0f,0.0f},
		{10.0f,10.0f,10.0f,1.0f},
	};

	//回転
	float rotation[4][4] = 
	{
	  {1.0f, 0.0f, 0.0f, 0.0f},
	  {0.0f, cos(PI/4),  sin(PI/4), 0.0f},
	  {0.0f, -sin(PI/4), cos(PI/4), 0.0f},
      {0.0f, 0.0f, 0.0f, 1.0f}
	};

	//拡大
	float scale[4][4] =
	{
	  {2.0f, 0.0f, 0.0f, 0.0f},
	  {0.0f, 2.0f, 0.0f, 0.0f},
	  {0.0f, 0.0f, 2.0f, 0.0f},
	  {0.0f, 0.0f, 0.0f, 1.0f},
	};

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};

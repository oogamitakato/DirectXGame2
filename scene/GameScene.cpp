#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "mathMyFunc.h"
#include <cassert>
#include <math.h>
#include <random>

//乱数シード生成器
std::random_device seed_gen;
//メルセンヌ・ツイスターの乱数エンジン
std::mt19937_64 engine(seed_gen());
//乱数範囲(回転角用)
std::uniform_real_distribution<float> rotDist(0.0f, PI * 2);
//乱数範囲(座標用)
std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);
//乱数範囲(スケーリング用)
std::uniform_real_distribution<float> sclDist(0.1f, 1.0f);

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete debugCamera_;

	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	// 3Dモデル生成
	model_ = Model::Create();

	//カメラ視点座標を設定
	viewProjection_.eye = {0.0f, 0.0f, -50.0f};
	//カメラ注視点座標を設定
	viewProjection_.target = {0.0f, 0.0f, 0.0f};
	//カメラ上方向ベクトルを設定
	viewProjection_.up = {0.0f, 1.0f, 0.0f};

	////カメラの垂直方向視野角を設定
	// viewProjection_.fovAngleY = ConversionRadian(10.0f);
	////アスペクト比を設定
	// viewProjection_.aspectRatio = (float)16 / 9;
	////ニアクリップ距離を設定
	// viewProjection_.nearZ = 52.0f;
	////ファークリップ距離を設定
	// viewProjection_.farZ = 53.0f;

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	//ワールドトランスフォームの初期化
	//親
	worldTransforms_[0].Initialize();
	//子
	worldTransforms_[1].Initialize();
	worldTransforms_[1].translation_ = {0.0f, 4.5f, 0.0f};
	worldTransforms_[1].parent_ = &worldTransforms_[0];
}

void GameScene::Update() {
	//キャラクター移動処理
	{
		//キャラクターの移動ベクトル
		Vector3 move = {0, 0, 0};

		//キャラクターの移動速さ
		const float kCharacterSpeed = 0.2f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_LEFT)) {
			move = {-kCharacterSpeed, 0.0f, 0.0f};
		} else if (input_->PushKey(DIK_RIGHT)) {
			move = {kCharacterSpeed, 0.0f, 0.0f};
		}

		// translationをmoveに加算
		worldTransforms_[0].translation_ += move;

		//ワールド行列を計算
		//移動量を行列に設定する
		matTrans = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f,   1.0f,   0.0f,   0.0f,
		            0.0f, 0.0f, 1.0f, 0.0f, move.x, move.y, move.z, 1.0f};

		// matTransを掛け算して代入
		worldTransforms_[0].matWorld_ *= matTrans;

		//ワールド行列を転送
		worldTransforms_[0].TransferMatrix();

		//デバッグ用表示
		debugText_->SetPos(50, 50);
		debugText_->Printf(
		  "Root : (%f,%f,%f)", worldTransforms_[0].matWorld_.m[3][0],
		  worldTransforms_[0].matWorld_.m[3][1], worldTransforms_[0].matWorld_.m[3][2]);
	}

	//子の更新
	{
		

		//ワールド行列を掛け算代入
		worldTransforms_[1].matWorld_ *= worldTransforms_[0].matWorld_;

		//ワールド行列を転送
		worldTransforms_[1].TransferMatrix();
	}

	//行列の再計算
	viewProjection_.UpdateMatrix();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dモデル描画
	model_->Draw(worldTransforms_[0], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[1], viewProjection_, textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::SetScale(float x, float y, float z, int i) {

	// X,Y,Z方向のスケーリングを設定
	worldTransforms_[i].scale_ = {x, y, z};
	//スケーリング行列を宣言
	Matrix4 matScale;

	//スケーリング倍率を行列に設定
	matScale = {
	  worldTransforms_[i].scale_.x,
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  worldTransforms_[i].scale_.y,
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  worldTransforms_[i].scale_.z,
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  1.0f};

	// matScaleを掛け算して代入
	worldTransforms_[i].matWorld_ *= matScale;
}

void GameScene::SetRot(float x, float y, float z, int i) {

	// X,Y,Z軸周りの回転角を設定
	worldTransforms_[i].rotation_ = {x, y, z};
	//合成用回転行列を宣言
	Matrix4 matRot;
	//各軸回転行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;

	// Z軸回転行列を宣言
	matRotZ = {
	  cos(worldTransforms_[i].rotation_.z),
	  sin(worldTransforms_[i].rotation_.z),
	  0.0f,
	  0.0f,
	  -sin(worldTransforms_[i].rotation_.z),
	  cos(worldTransforms_[i].rotation_.z),
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
	  cos(worldTransforms_[i].rotation_.x),
	  sin(worldTransforms_[i].rotation_.x),
	  0.0f,
	  0.0f,
	  -sin(worldTransforms_[i].rotation_.x),
	  cos(worldTransforms_[i].rotation_.x),
	  0.0f,
	  0.0f,
	  0.0f,
	  0.0f,
	  1.0f};

	// Y軸回転行列を宣言
	matRotY = {
	  cos(worldTransforms_[i].rotation_.y),
	  0.0f,
	  -sin(worldTransforms_[i].rotation_.y),
	  0.0f,
	  0.0f,
	  1.0f,
	  0.0f,
	  0.0f,
	  sin(worldTransforms_[i].rotation_.y),
	  0.0f,
	  cos(worldTransforms_[i].rotation_.y),
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
	worldTransforms_[i].matWorld_ *= matRot;
}

void GameScene::SetTrans(float x, float y, float z, int i) {

	// X,Y,Z軸周りの平行移動を設定
	worldTransforms_[i].translation_ = {x, y, z};
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
	  worldTransforms_[i].translation_.x,
	  worldTransforms_[i].translation_.y,
	  worldTransforms_[i].translation_.z,
	  1.0f};

	// matTransを掛け算して代入
	worldTransforms_[i].matWorld_ *= matTrans;
}
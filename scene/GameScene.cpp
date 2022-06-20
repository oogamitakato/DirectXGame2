#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>
#include <math.h>
#include <random>

GameScene::GameScene() {}

GameScene::~GameScene() {

	//自キャラの解放
	delete player_;

	delete debugCamera_;

	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールドトランスフォームの初期化

	//カメラ視点座標を設定
	viewProjection_.eye = {0, 0, -50};

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	////乱数範囲(回転角用)
	//std::uniform_real_distribution<float> rotDist(0.0f, PI * 2);
	////乱数範囲(座標用)
	//std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);
	////乱数範囲(スケーリング用)
	//std::uniform_real_distribution<float> scaDist(0.1f, 5.0f);

	//float rotRand = rotDist(engine);
	//float posRand = posDist(engine);
	//float scaRand = scaDist(engine);

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	// 3Dモデル生成
	model_ = Model::Create();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向h表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	//自キャラの生成
	player_ = new Player(model_, textureHandle_);

}

void GameScene::Update() { 

	//自キャラの更新
	player_->Update();
	debugCamera_->Update();
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

	//自キャラの描画
	player_->Draw(viewProjection_);

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

void GameScene::SetScale(float& x, float& y, float& z) {

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

void GameScene::SetRot(float& x, float& y, float& z) {

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

void GameScene::SetTrans(float x, float y, float z) {

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


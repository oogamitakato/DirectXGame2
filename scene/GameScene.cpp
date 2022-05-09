#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"

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

	//3Dモデル生成
	model_ = Model::Create();

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向h表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	for (int i = 0; i < 8; i++) 
	{
		//平行移動する立方体の初期化
		boxMoved[i] = vertex[i];
		boxMoved2[i] = boxMoved[i];

		boxMoved[i] = {
		  boxMoved2[i].x * position[0][0] + boxMoved2[i].y * position[1][0] + boxMoved2[i].z * position[2][0] + position[3][0],
		  boxMoved2[i].x * position[0][1] + boxMoved2[i].y * position[1][1] + boxMoved2[i].z * position[2][1] + position[3][1],
		  boxMoved2[i].x * position[0][2] + boxMoved2[i].y * position[1][2] + boxMoved2[i].z * position[2][2] + position[3][2],
		  
		};

		//回転する立方体の初期化
		boxRotated[i] = vertex[i];
		boxRotated2[i] = boxRotated[i];

		boxRotated[i] = {
		  boxRotated2[i].x * rotation[0][0] + boxRotated2[i].y * rotation[1][0] + boxRotated2[i].z * rotation[2][0],
		  boxRotated2[i].x * rotation[0][1] + boxRotated2[i].y * rotation[1][1] + boxRotated2[i].z * rotation[2][1],
		  boxRotated2[i].x * rotation[0][2] + boxRotated2[i].y * rotation[1][2] + boxRotated2[i].z * rotation[2][2],
		};

		//拡大する立方体の初期化
		boxScaled[i] = vertex[i];
		boxScaled2[i] = boxScaled[i];

		boxScaled[i] = {
		  boxScaled2[i].x * scale[0][0] + boxScaled2[i].y * scale[1][0] + boxScaled2[i].z * scale[2][0],
		  boxScaled2[i].x * scale[0][1] + boxScaled2[i].y * scale[1][1] + boxScaled2[i].z * scale[2][1],
		  boxScaled2[i].x * scale[0][2] + boxScaled2[i].y * scale[1][2] + boxScaled2[i].z * scale[2][2],
		};
	}


}

void GameScene::Update() { 

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

	//3Dモデル描画
	//model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	for (int i = 0; i < 12; i++) 
	{
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		  vertex[eageList[i][0]], vertex[eageList[i][1]], Vector4(255, 255, 255, 255));
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		  boxMoved[eageList[i][0]], boxMoved[eageList[i][1]], Vector4(255, 0, 0, 255));
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		  boxRotated[eageList[i][0]], boxRotated[eageList[i][1]], Vector4(0, 255, 0, 255));
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		  boxScaled[eageList[i][0]], boxScaled[eageList[i][1]], Vector4(0, 0, 255, 255));
	}


	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

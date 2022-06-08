#include "GameScene.h"
#include "TextureManager.h"
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"
#include "mathMyFunc.h"
#include <math.h>
#include <cassert>
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

	//3Dモデル生成
	model_ = Model::Create();

	//カメラ視点座標を設定
	viewProjection_.eye = {0.0f, 0.0f, -50.0f};
	//カメラ注視点座標を設定
	viewProjection_.target = {0.0f, 0.0f, 0.0f};
	//カメラ上方向ベクトルを設定
	viewProjection_.up = {0.0f, 1.0f, 0.0f};

	////カメラの垂直方向視野角を設定
	//viewProjection_.fovAngleY = ConversionRadian(10.0f);
	////アスペクト比を設定
	//viewProjection_.aspectRatio = (float)16 / 9;
	////ニアクリップ距離を設定
	//viewProjection_.nearZ = 52.0f;
	////ファークリップ距離を設定
	//viewProjection_.farZ = 53.0f;
	
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

	//for (WorldTransform& worldTransform : worldTransforms_) {

	//	//乱数エンジンを渡し、指定範囲からランダムな数値を得る
	//	float rotRand = rotDist(engine);
	//	float posXRand = posDist(engine);
	//	float posYRand = posDist(engine);
	//	float posZRand = posDist(engine);
	//	float sclRand = sclDist(engine);
	//	
	//}
	
	for (WorldTransform& worldTransform : worldTransforms_) {
		//	ワールドトランスフォームの初期化
		worldTransform.Initialize();

		//単位行列を代入
		worldTransform.matWorld_.m;
		//スケーリング
		SetScale(1.0f, 1.0f, 1.0f);

		//回転
		SetRot(0.0f, 0.0f, 0.0f);

		//平行移動
		SetTrans(0.0f, 0.0f, 0.0f);

		//行列の転送
		worldTransform.TransferMatrix();
	}


}

void GameScene::Update() { 
	//視点移動処理
	{
		////視点の移動べクトル
		//Vector3 move = {0.0f, 0.0f, 0.0f};

		////視点の移動速さ
		//const float kEyeSpeed = 0.2f;

		////押した方向で移動ベクトルを変更
		//if (input_->PushKey(DIK_W)) {
		//	move.z += kEyeSpeed;
		//} else if (input_->PushKey(DIK_S)) {
		//	move.z -= kEyeSpeed;
		//}

		////視点移動(ベクトルの加算)
		//viewProjection_.eye += move;

		//デバッグ用表示
		debugText_->SetPos(50, 50);
		debugText_->Printf(
		  "eye:(%f,%f,%f)",
			viewProjection_.eye.x,
			viewProjection_.eye.y,
			viewProjection_.eye.z);
	}

	//注視点移動処理
	{
		////注視点の移動ベクトル
		//Vector3 move = {0.0f, 0.0f, 0.0f};

		////注視点の移動速さ
		//const float kTargetSpeed = 0.2f;

		////押した方向で移動ベクトルを変更
		//if (input_->PushKey(DIK_LEFT)) {
		//	move.x -= kTargetSpeed;
		//} else if (input_->PushKey(DIK_RIGHT)) {
		//	move.x += kTargetSpeed;
		//}

		////注視点移動(ベクトルの加算)
		//viewProjection_.target += move;

		//デバッグ用表示
		debugText_->SetPos(50, 70);
		debugText_->Printf(
		  "target:(%f,%f,%f)",
			viewProjection_.target.x,
			viewProjection_.target.y,
			viewProjection_.target.z);
	}

	//上方向回転処理
	{
		////上方向の回転速さ[ラジアン/flame]
		//const float kUpRotSpeed = 0.05f;

		////押した方向で移動ベクトルを変更
		//if (input_->PushKey(DIK_SPACE)) {
		//	viewAngle += kUpRotSpeed;
		//	//2πを超えたら0に戻す
		//	viewAngle = fmodf(viewAngle, PI * 2.0f);
		//}

		////上方向ベクトルを計算(半径1の円周上の座標)
		//viewProjection_.up = {cosf(viewAngle), sinf(viewAngle), 0.0f};

		//デバッグ用表示
		debugText_->SetPos(50, 90);
		debugText_->Printf(
		  "up:(%f,%f,%f)",
			viewProjection_.up.x,
			viewProjection_.up.y,
			viewProjection_.up.z);
	}

	//Fov変更処理
	{
		//上キーで視野角が広がる
		if (input_->PushKey(DIK_UP)) {
			viewProjection_.fovAngleY += 0.01f;
			//πを上回らないようにする
			viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, PI);
		//下キーで視野角が狭まる
		} else if (input_->PushKey(DIK_DOWN)) {
			viewProjection_.fovAngleY -= 0.01f;
			//0.01fを下回らないようにする
			viewProjection_.fovAngleY = max(viewProjection_.fovAngleY, 0.01f);
		}

		//デバッグ用表示
		debugText_->SetPos(50, 110);
		debugText_->Printf(
		  "fovAngleY(Degree):%f", ConversionDegree(viewProjection_.fovAngleY));
	}

	//クリップ距離更新処理
	{
		//W,Sキーでニアクリップを増減
		if (input_->PushKey(DIK_W)) {
			viewProjection_.nearZ += 0.1f;
		} else if (input_->PushKey(DIK_S)) {
			viewProjection_.nearZ -= 0.1f;
		}

		//デバッグ用表示
		debugText_->SetPos(50, 130);
		debugText_->Printf("nearZ:%f",viewProjection_.nearZ);
	}

	//行列の再計算
	viewProjection_.UpdateMatrix();


	//debugCamera_->Update();

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
	for (WorldTransform& worldTransform : worldTransforms_) {	
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
	}

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

void GameScene::SetScale(float x, float y, float z) {

	for (WorldTransform& worldTransform : worldTransforms_) {
		// X,Y,Z方向のスケーリングを設定
		worldTransform.scale_ = {x, y, z};
		//スケーリング行列を宣言
		Matrix4 matScale;

		//スケーリング倍率を行列に設定
		matScale = {
			worldTransform.scale_.x,0.0f,0.0f,0.0f,
			0.0f,worldTransform.scale_.y,0.0f,0.0f,
			0.0f,0.0f,worldTransform.scale_.z,0.0f,
			0.0f,0.0f,0.0f,1.0f
		};

		// matScaleを掛け算して代入
		worldTransform.matWorld_ *= matScale;
	}
}

void GameScene::SetRot(float x, float y, float z) {

	for (WorldTransform& worldTransform : worldTransforms_) {
		// X,Y,Z軸周りの回転角を設定
		worldTransform.rotation_ = {x, y, z};
		//合成用回転行列を宣言
		Matrix4 matRot;
		//各軸回転行列を宣言
		Matrix4 matRotX, matRotY, matRotZ;

		// Z軸回転行列を宣言
		matRotZ = {
			cos(worldTransform.rotation_.z),sin(worldTransform.rotation_.z),0.0f,0.0f,
			-sin(worldTransform.rotation_.z),cos(worldTransform.rotation_.z),0.0f,0.0f,
			0.0f,0.0f,1.0f,0.0f,
			0.0f,0.0f,0.0f,1.0f
		};

		// X軸回転行列を宣言
		matRotX = {
		  1.0f,0.0f,0.0f,0.0f,
			0.0f,cos(worldTransform.rotation_.x),sin(worldTransform.rotation_.x),0.0f,
			0.0f,-sin(worldTransform.rotation_.x),cos(worldTransform.rotation_.x),0.0f,
			0.0f,0.0f,0.0f,1.0f
		};

		// Y軸回転行列を宣言
		matRotY = {
			cos(worldTransform.rotation_.y),0.0f,-sin(worldTransform.rotation_.y),0.0f,
			0.0f,1.0f,0.0f,0.0f,
			sin(worldTransform.rotation_.y),0.0f,cos(worldTransform.rotation_.y),0.0f,
			0.0f,0.0f,0.0f,1.0f
		};

		//各軸の回転行列を合成
		matRotZ *= matRotX;
		matRotZ *= matRotY;
		matRot = matRotZ;

		// matRotを掛け算して代入
		worldTransform.matWorld_ *= matRot;
	}
}

void GameScene::SetTrans(float x, float y, float z) {

	for (WorldTransform& worldTransform : worldTransforms_) {
		// X,Y,Z軸周りの平行移動を設定
		worldTransform.translation_ = {x, y, z};
		//平行移動行列を宣言
		Matrix4 matTrans = MathUtility::Matrix4Identity();

		//移動量を行列に設定する
		matTrans = {
			1.0f,0.0f,0.0f,0.0f,
			0.0f,1.0f,0.0f,0.0f,
			0.0f,0.0f,1.0f,0.0f,
			worldTransform.translation_.x,worldTransform.translation_.y,worldTransform.translation_.z,1.0f
		};

		// matTransを掛け算して代入
		worldTransform.matWorld_ *= matTrans;
	}
}
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
	//敵の解放
	delete enemy_;
	delete debugCamera_;

	delete model_;
	//天球の解放
	delete skydome_;
	delete modelSkydome_;

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
	//乱数範囲(回転角用)
	std::uniform_real_distribution<float> rotDist(0.0f, PI * 2);
	//乱数範囲(座標用)
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);
	//乱数範囲(スケーリング用)
	std::uniform_real_distribution<float> scaDist(0.1f, 5.0f);

	float rotRand = rotDist(engine);
	float posRand = posDist(engine);
	float scaRand = scaDist(engine);

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	// 3Dモデル生成
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

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

	//敵の生成
	enemy_ = new Enemy(model_);

	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	//天球の生成
	skydome_ = new Skydome();
	//天球の初期化
	skydome_->Initialize(modelSkydome_, textureHandle_);
}

void GameScene::Update() {

	//自キャラの更新
	player_->Update();

	//敵の更新
	if (enemy_) {
		enemy_->Update();
	}

	//当たり判定
	CheckAllCollisions();

	debugCamera_->Update();

	//天球の更新
	skydome_->Update();
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

	if (enemy_) {
		enemy_->Draw(viewProjection_);
	}

	//天球の描画
	skydome_->Draw(viewProjection_);

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

void GameScene::CheckAllCollisions() {
	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();

		//座標AとBの距離を求める
		float distance =
		  pow(posA.x - posB.x, 2.0f) + pow(posA.y - posB.y, 2.0f) + pow(posA.z - posB.z, 2.0f);

		//当たり判定の半径を設定
		float radianA = 1.0f;
		float radianB = 1.0f;

		float collision = pow(radianA + radianB, 2.0f);

		//球と球の交差判定
		if (distance <= collision) {
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	//敵キャラの座標
	posA = enemy_->GetWorldPosition();

	//自弾全てと敵キャラの当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();

		//座標AとBの距離を求める
		float distance =
		  pow(posA.x - posB.x, 2.0f) + pow(posA.y - posB.y, 2.0f) + pow(posA.z - posB.z, 2.0f);

		//当たり判定の半径を設定
		float radianA = 1.0f;
		float radianB = 1.0f;

		float collision = pow(radianA + radianB, 2.0f);

		//球と球の交差判定
		if (distance <= collision) {
			//敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			//自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	//自弾全てと敵キャラの当たり判定
	for (const std::unique_ptr<PlayerBullet>& pBullet : playerBullets) {
		for (const std::unique_ptr<EnemyBullet>& eBullet : enemyBullets) {

			//自弾の座標
			posA = pBullet->GetWorldPosition();

			//敵弾の座標
			posB = eBullet->GetWorldPosition();

			//座標AとBの距離を求める
			float distance =
			  pow(posA.x - posB.x, 2.0f) + pow(posA.y - posB.y, 2.0f) + pow(posA.z - posB.z, 2.0f);

			//当たり判定の半径を設定
			float radianA = 1.0f;
			float radianB = 0.5f;

			float collision = pow(radianA + radianB, 2.0f);

			//球と球の交差判定
			if (distance <= collision) {
				//自弾の衝突時コールバックを呼び出す
				pBullet->OnCollision();
				//敵弾の衝突時コールバックを呼び出す
				eBullet->OnCollision();
			}
		}
	}
#pragma endregion
}
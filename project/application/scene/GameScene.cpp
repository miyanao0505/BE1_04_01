#include "GameScene.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "ParticleManager.h"
#include "AudioManager.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "MyTools.h"
#ifdef _DEBUG
#include "DebugLineBase.h"
#endif // _DEBUG

using namespace std;
using namespace MyBase;

// 初期化
void GameScene::Initialize() {
#pragma region シーン初期化
	BaseScene::Initialize();

#pragma region ライト
	LightManager::GetInstance()->SetPointLightIntensity(0.0f);
	LightManager::GetInstance()->SetDirectionalLightIntensity(1.0f);
	LightManager::GetInstance()->SetSpotLightIntensity(0.0f);
#pragma endregion ライト

#pragma region スプライト
	
#pragma endregion スプライト

#pragma region 3Dオブジェクト

#pragma endregion 3Dオブジェクト

#pragma region カメラ

#pragma endregion カメラ

#pragma region デバッグ
#ifdef _DEBUG
	DebugLineBase::GetInstance()->Initialize(DirectXBase::GetInstance());
#endif // _DEBUG
#pragma endregion

#pragma region パーティクル
	// パーティクル
	
#pragma endregion パーティクル

#pragma region jsonローダー
	// jsonローダー
	jsonLoader_ = make_unique<JsonLoader>();
	LoadJsonFile("gameScene.json");
#pragma endregion jsonローダー

#pragma region オーディオ
	// BGM

	// お試し用
	AudioManager::GetInstance()->LoadAudioWave("fanfare.wav");
#pragma endregion オーディオ

	// 最初の更新
	CameraManager::GetInstance()->GetCamera()->SetTranslate(kCameraTranslate);
	CameraManager::GetInstance()->GetCamera()->Update();
#pragma endregion シーン初期化
}

// 終了
void GameScene::Finalize() {
	jsonLoader_.reset();

	// 3Dオブジェクト
	

	// スプライト
	
	
#ifdef _DEBUG
	DebugLineBase::Finalize();
#endif // _DEBUG

	BaseScene::Finalize();
}

// 毎フレーム更新
void GameScene::Update()
{
	BaseScene::Update();

#ifdef _DEBUG
	// デバッグ更新
	DebugUpdate();
#endif // _DEBUG

	// カメラマネージャーの更新
	CameraManager::GetInstance()->Update(TimeManager::GetInstance()->GetDeltaTime());
	// カメラの更新
	if (CameraManager::GetInstance()->GetCamera()) {
		CameraManager::GetInstance()->GetCamera()->Update();
	}

	// 3Dオブジェクトの更新処理


	// パーティクルの更新処理
	ParticleManager::GetInstance()->Update();

	// スプライトの更新処理
	
}

// 描画
void GameScene::Draw() {
#pragma region 3Dオブジェクト

	// 3Dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	ModelManager::GetInstance()->SetCommonScreen();

	// 全ての3DObject個々の描画


#pragma endregion 3Dオブジェクト

#pragma region パーティクル

	// パーティクルの描画準備。パーティクルの描画に共通グラフィックスコマンドを積む
	ParticleManager::GetInstance()->Draw();

#pragma endregion パーティクル

#pragma region デバッグライン
#ifdef _DEBUG
	DebugLineManager::GetInstance()->DrawAll();
#endif // _DEBUG
#pragma endregion デバッグライン

#pragma region スプライト

	// Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	TextureManager::GetInstance()->SetCommonScreen();

	// 全てのSprite個々の描画
	

#pragma endregion スプライト
}

#ifdef _DEBUG
// デバッグ更新
void GameScene::DebugUpdate() {
	DebugDraw();
}

// デバッグ描画
void GameScene::DebugDraw() {
	// 開発用UIの処理。実際に開発用のUIを出す場合はここをゲーム固有の処理に置き換える
	ImGui::SetNextWindowPos(kDebugWindowPosSettings, ImGuiCond_Once);		// ウィンドウの座標(プログラム起動時のみ読み込み)
	ImGui::SetNextWindowSize(kDebugWindowSizeSettings, ImGuiCond_Once);		// ウィンドウのサイズ(プログラム起動時のみ読み込み)

	ImGui::Begin("Settings");
	// Camera
	CameraManager::GetInstance()->DebugDraw();

	// Lighting
	LightManager::GetInstance()->DebugDraw();

	// パーティクル
	ParticleManager::GetInstance()->ImGui();

	ImGui::End();
}
#endif // _DEBUG

// JSONファイルの読み込み
void GameScene::LoadJsonFile([[maybe_unused]] const string& filePath) {
	// レベルデータの読み込み
	unique_ptr<JsonLevelData> levelData = jsonLoader_->LoadFile(filePath);

	// 3Dオブジェクトの読み込み
	for (const JsonObjectData& objectData : levelData->objects) {
		objectData;
	}
}
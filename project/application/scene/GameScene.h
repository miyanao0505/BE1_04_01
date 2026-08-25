#pragma once
#include "BaseScene.h"
#include <string>
#include <vector>
#include "Sprite.h"
#include "ParticleEmitter.h"
#include "MyBase.h"
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

/// <summary>
/// プレイヤーが実際に操作し、敵との戦闘や進行を行うゲーム本編のメインシーンを管理するクラス。
/// BaseSceneを継承し、ゲームプレイ中のキャラクター制御、カメラ挙動、敵出現、演出などを統括します。
/// </summary>
class GameScene : public BaseScene
{
private:
#pragma region 定数
	// カメラ初期設定
	static constexpr MyBase::Vector3 kCameraTranslate{ 0.0f, 0.0f,0.0f };
	static constexpr MyBase::Vector3 kCameraRotate{ 0.0f, 0.0f, 0.0f };

#ifdef _DEBUG
	// ImGui
	static constexpr ImVec2 kDebugWindowPosScene{ 20.0f, 350.0f };
	static constexpr ImVec2 kDebugWindowSizeScene{ 350.0f, 150.0f };
	static constexpr ImVec2 kDebugWindowPosSettings{ 900.0f, 20.0f };
	static constexpr ImVec2 kDebugWindowSizeSettings{ 350.0f, 150.0f };
#endif // _DEBUG
#pragma endregion

public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

#ifdef _DEBUG
	/// <summary>
	/// デバッグ更新
	///	</summary>
	void DebugUpdate();

	/// <summary>
	/// デバッグ描画
	/// </summary>
	void DebugDraw() override;
#endif // _DEBUG

	/// <summary>
	/// jsonファイルの読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void LoadJsonFile([[maybe_unused]] const std::string& filePath) override;

private:	// メンバ変数
#pragma region スプライト

#pragma endregion

#pragma region 3Dオブジェクト

#pragma endregion

	// パーティクル

	// jsonローダー
	std::unique_ptr<JsonLoader> jsonLoader_ = nullptr;
};
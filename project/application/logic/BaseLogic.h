#pragma once
#include <memory>
#include "Input.h"

/// 前方宣言
class LogicManager;

/// <summary>
/// ロジック基底クラス(BaseLogic)
/// ゲームロジック(ゲーム内の処理やルールなど)
/// の共通インターフェースと基本機能を提供する抽象クラスです。
/// </summary>
class BaseLogic
{
public:	// メンバ関数
	virtual ~BaseLogic() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() {}

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize() {}

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() {}

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() {}

#ifdef _DEBUG
	/// <summary>
	/// デバッグ描画
	/// </summary>
	virtual void DebugDraw();
#endif // _DEBUG

public:	// setter
	/// <summary>
	/// ロジックマネージャーのセット
	/// </summary>
	/// <param name="logicManager">ロジックマネージャーのポインタ</param>
	virtual void SetLogicManager(LogicManager* logicManager) { logicManager_ = logicManager; }

private:	// メンバ変数
	// ロジックマネージャー(借りてくる)
	LogicManager* logicManager_ = nullptr;
	// 入力
	Input* input_ = nullptr;
};
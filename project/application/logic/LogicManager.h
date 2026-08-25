#pragma once
#include "BaseLogic.h"
#include "AbstractLogicFactory.h"

/// <summary>
/// ゲームロジック管理クラス
/// ロジックの生成・更新・描画・切り替えを管理するマネージャークラス。
/// </summary>
class LogicManager {
public:	// メンバ関数[
	/// <summary>
	/// Singleton Instance を取得
	/// </summary>
	/// <returns>LogicManager</returns>
	static LogicManager* GetInstance();

	/// ------ Passkey Idion ------
	/// コンストラクタを渡すための鍵
	class ConstructorKey {
	private:
		ConstructorKey() = default;
		friend class LogicManager;
	};

	/// PassKeyを受け取るコンストラクタ	
	explicit LogicManager(ConstructorKey) {}

	/// コピー禁止
	LogicManager(const LogicManager&) = delete;
	LogicManager& operator=(const LogicManager&) = delete;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 次のロジック予約
	/// </summary>
	/// <param name="logicName">ロジック名</param>
	void ChangeLogic(LogicName logicName);
	
public:	// setter
	/// <summary>
	/// ロジックファクトリーのセット
	/// </summary>
	/// <param name="logicFactory">ロジックファクトリーのポインタ</param>
	void SetLogicFactory(AbstractLogicFactory* logicFactory) { logicFactory_ = logicFactory; }

private:	// Singleton Instance
	static std::unique_ptr<LogicManager> sInstance_;

private:	// メンバ変数
	// 現在のロジック
	std::unique_ptr<BaseLogic> logic_;
	
	// 次のロジック
	std::unique_ptr<BaseLogic> nextLogic_;

	// ロジックファクトリー(借りてくる)
	AbstractLogicFactory* logicFactory_ = nullptr;
};
#include "LogicManager.h"
#include <cassert>
#include <imgui.h>

using namespace std;

/// static member 定義
unique_ptr<LogicManager> LogicManager::sInstance_ = nullptr;

/// Singleton Instance を取得
LogicManager* LogicManager::GetInstance() {
	if (sInstance_ == nullptr) {
		sInstance_ = make_unique<LogicManager>(LogicManager::ConstructorKey{});
	}
	return sInstance_.get();
}

/// 終了
void LogicManager::Finalize() {
	logic_->Finalize();
	logic_.reset();

	sInstance_.reset();
}

/// 更新
void LogicManager::Update() {
	// 起動直後（logic_がnullptr）の場合は即ロジック切り替え
	if (!logic_ && nextLogic_) {
		logic_ = std::move(nextLogic_);
		logic_->SetLogicManager(this);
		logic_->Initialize();
		return;
	}

	// 次のロジックが予約されている場合は切り替え
	if (nextLogic_) {
		logic_.reset();		// 古いロジックを破棄
		logic_ = std::move(nextLogic_);
		logic_->SetLogicManager(this);
		logic_->Initialize();
		return;
	}

	// ロジックの更新
	if (logic_) {
		logic_->Update();
	}
}

/// 描画
void LogicManager::Draw() {
	if (logic_) {
		logic_->Draw();
	}
}

/// 次のロジック予約
void LogicManager::ChangeLogic(LogicName logicName) {
	assert(logicFactory_);
	assert(!nextLogic_);

	// 次のロジックを生成
	nextLogic_ = logicFactory_->CreateLogic(logicName);
}
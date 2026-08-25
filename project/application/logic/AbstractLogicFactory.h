#pragma once
#include "BaseLogic.h"
#include "LogicName.h"

/// <summary>
/// ロジックファクトリー抽象クラス(AbstractLogicFactory)
/// ゲーム内のロジックを生成するためのインターフェースを定義する抽象クラスです。
/// </summary>
class AbstractLogicFactory {
public:	// メンバ関数
		// 仮想デストラクタ
		virtual ~AbstractLogicFactory() = default;

		/// <summary>
		/// ロジックの生成
		/// </summary>
		/// <param name="logicName">ロジック名</param>
		/// <returns>生成したロジックのポインタ</returns>
		virtual std::unique_ptr<BaseLogic> CreateLogic(LogicName logicName) = 0;
};
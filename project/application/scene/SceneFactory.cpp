#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"

SceneFactory::SceneFactory() {
    createTable_.emplace(SceneName::Title, [] { return std::make_unique<TitleScene>(); });
    createTable_.emplace(SceneName::Game, [] { return std::make_unique<GameScene>(); });
}

/// SceneNameに対応するシーンインスタンスを生成する
std::unique_ptr<BaseScene> SceneFactory::CreateScene(SceneName sceneName) {
	auto it = createTable_.find(sceneName);
	if (it == createTable_.end()) {
		return nullptr;
	}

	return it->second();
}

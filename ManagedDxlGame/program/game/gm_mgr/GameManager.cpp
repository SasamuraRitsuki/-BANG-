#include "../../dxlib_ext/dxlib_ext.h"
#include "GameManager.h"
#include "SceneBase.h"
#include "ResourceManager.h"

GameManager::GameManager(std::shared_ptr<SceneBase> start_scene):now_scene_(start_scene) {
	//始めに重いbgmデータをロードしておく
	int a = ResourceManager::GetInstance()->loadSound("sound/bgm/battle_bgm.mp3");
	int b = ResourceManager::GetInstance()->loadSound("sound/bgm/menu_bgm.mp3");
}

GameManager::~GameManager() {

}

GameManager* GameManager::GetInstance(std::shared_ptr<SceneBase> start_scene) {
	static GameManager* gm = nullptr;
	if (!gm) {
		gm = new GameManager(start_scene);
	}
	return gm;
}

void GameManager::Destroy() {
	delete GetInstance();
}

// next_scene_をセット
void GameManager::changeScene(std::shared_ptr<SceneBase> next_scene) {
	next_scene_ = next_scene;
}


void GameManager::update(float delta_time) {

	if (now_scene_) {
		now_scene_->update(delta_time);
		now_scene_->draw(); 
	}

	if (next_scene_) {
		now_scene_ = next_scene_;
		next_scene_ = nullptr;
	}

}

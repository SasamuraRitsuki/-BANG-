#include "../dxlib_ext/dxlib_ext.h"
#include "GameMain.h"
#include "gm_mgr/GameManager.h"
#include "scene/scene_title/GameSceneTitle.h"
#include "scene/scene_play/GameScenePlay.h"

//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
void gameStart() {
	srand(static_cast<unsigned int>(time(0)));

	SetDefaultLightParameter("directional_light_parameter.bin");

	GameManager::GetInstance(std::make_shared<SceneTitle>());
	//GameManager::GetInstance(std::make_shared<ScenePlay>());

	SetFontSize(50);
}
//------------------------------------------------------------------------------------------------------------
// 毎フレーム実行されます
void gameMain(float delta_time) {

	GameManager::GetInstance()->update(delta_time);

	//DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);

	/*SetFontSize(20);
	DrawDefaultLightGuiController();*/
}

//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {
	GameManager::Destroy();
}

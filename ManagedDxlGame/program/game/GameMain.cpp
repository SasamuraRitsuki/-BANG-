#include "../dxlib_ext/dxlib_ext.h"
#include "GameMain.h"
#include "gm_mgr/GameManager.h"
#include "scene/scene_title/GameSceneTitle.h"
#include "scene/scene_play/GameScenePlay.h"

//------------------------------------------------------------------------------------------------------------
// �Q�[���N�����ɂP�x�������s����܂�
void gameStart() {
	srand(static_cast<unsigned int>(time(0)));

	SetDefaultLightParameter("directional_light_parameter.bin");

	GameManager::GetInstance(std::make_shared<SceneTitle>());
	//GameManager::GetInstance(std::make_shared<ScenePlay>());

	SetFontSize(50);
}
//------------------------------------------------------------------------------------------------------------
// ���t���[�����s����܂�
void gameMain(float delta_time) {

	GameManager::GetInstance()->update(delta_time);

	//DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);

	/*SetFontSize(20);
	DrawDefaultLightGuiController();*/
}

//------------------------------------------------------------------------------------------------------------
// �Q�[���I�����ɂP�x�������s����܂�
void gameEnd() {
	GameManager::Destroy();
}

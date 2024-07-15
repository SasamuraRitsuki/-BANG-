#include "../../../../dxlib_ext/dxlib_ext.h"
#include "Result.h"
#include "../../scene_title/GameSceneTitle.h"
#include "../../../gm_mgr/GameManager.h"
#include "../../../gm_mgr/ResourceManager.h"

SceneResult::SceneResult(bool cleared) {
	//�N���A�̔�����󂯎��
	cleared_ = cleared;
	//�摜��ǂݍ���
	back_gfx_ = ResourceManager::GetInstance()->loadGraph("graphics/black2.jpg");

	//�Q�[���N���A��
	if (cleared_) {
		finish_snd_ = ResourceManager::GetInstance()->loadSound("sound/se/game_clear.mp3");
	}
	//�Q�[���I�[�o�[��
	else {
		finish_snd_ = ResourceManager::GetInstance()->loadSound("sound/se/enemy_dead.mp3");
	}
	enter_snd_ = ResourceManager::GetInstance()->loadSound("sound/se/enter_se.mp3");
	//�Q�[���I�����̌��ʉ�
	PlaySoundMem(finish_snd_, DX_PLAYTYPE_BACK);
}

SceneResult::~SceneResult() {

}

void SceneResult::update(float delta_time) {
	timeleft_ += delta_time;
	elapsed_ = sin(timeleft_ * 5);

	//���N���b�N�Ń^�C�g����ʂ�
	if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_LEFT)) {
		//���ʉ��𗬂�
		PlaySoundMem(enter_snd_, DX_PLAYTYPE_BACK);
		auto mgr = GameManager::GetInstance();
		mgr->changeScene(std::make_shared<SceneTitle>());
	}
}

void SceneResult::draw() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, BACK_GFX_ALPHA);
	//���̍��������w�i�̕`��
	DrawExtendGraph(0, 0,
		DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, back_gfx_, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	//���ʂ̕����̕`��
	SetFontSize(RESULT_FONTSIZE);
	if (cleared_) {
		DrawStringEx(CLEAR_TEXT_POS.x, CLEAR_TEXT_POS.y, TEXT_COLOR, "�Q�[���N���A");
	}
	else {
		DrawStringEx(OVER_TEXT_POS.x, OVER_TEXT_POS.y, TEXT_COLOR, "�Q�[���I�[�o�[");
	}
	SetFontSize(NORMAL_FONTSIZE);
	if (elapsed_ > 0) {
		DrawStringEx(CLICK_TEXT_POS.x, CLICK_TEXT_POS.y, TEXT_COLOR, "���N���b�N���@�����ăl");
	}
}
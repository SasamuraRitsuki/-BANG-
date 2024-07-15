#include "../../../../dxlib_ext/dxlib_ext.h"
#include "Result.h"
#include "../../scene_title/GameSceneTitle.h"
#include "../../../gm_mgr/GameManager.h"
#include "../../../gm_mgr/ResourceManager.h"

SceneResult::SceneResult(bool cleared) {
	//クリアの判定を受け取る
	cleared_ = cleared;
	//画像を読み込む
	back_gfx_ = ResourceManager::GetInstance()->loadGraph("graphics/black2.jpg");

	//ゲームクリア時
	if (cleared_) {
		finish_snd_ = ResourceManager::GetInstance()->loadSound("sound/se/game_clear.mp3");
	}
	//ゲームオーバー時
	else {
		finish_snd_ = ResourceManager::GetInstance()->loadSound("sound/se/enemy_dead.mp3");
	}
	enter_snd_ = ResourceManager::GetInstance()->loadSound("sound/se/enter_se.mp3");
	//ゲーム終了時の効果音
	PlaySoundMem(finish_snd_, DX_PLAYTYPE_BACK);
}

SceneResult::~SceneResult() {

}

void SceneResult::update(float delta_time) {
	timeleft_ += delta_time;
	elapsed_ = sin(timeleft_ * 5);

	//左クリックでタイトル画面へ
	if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_LEFT)) {
		//効果音を流す
		PlaySoundMem(enter_snd_, DX_PLAYTYPE_BACK);
		auto mgr = GameManager::GetInstance();
		mgr->changeScene(std::make_shared<SceneTitle>());
	}
}

void SceneResult::draw() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, BACK_GFX_ALPHA);
	//後ろの黒い薄い背景の描画
	DrawExtendGraph(0, 0,
		DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, back_gfx_, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	//結果の文字の描画
	SetFontSize(RESULT_FONTSIZE);
	if (cleared_) {
		DrawStringEx(CLEAR_TEXT_POS.x, CLEAR_TEXT_POS.y, TEXT_COLOR, "ゲームクリア");
	}
	else {
		DrawStringEx(OVER_TEXT_POS.x, OVER_TEXT_POS.y, TEXT_COLOR, "ゲームオーバー");
	}
	SetFontSize(NORMAL_FONTSIZE);
	if (elapsed_ > 0) {
		DrawStringEx(CLICK_TEXT_POS.x, CLICK_TEXT_POS.y, TEXT_COLOR, "左クリックを　押してネ");
	}
}
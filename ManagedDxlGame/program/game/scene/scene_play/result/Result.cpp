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
	//点滅用の計算
	flashed_value_ = sin(timeleft_ * FLASHED_VALUE);

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
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, TRANSPARENCY_MAX);

	//結果の文字の描画
	SetFontSize(RESULT_FONTSIZE);
	if (cleared_) {
		//クリアテキストの表示
		DrawStringEx(CLEAR_TEXT_POS.x, CLEAR_TEXT_POS.y, TEXT_COLOR, CLEAR_TEXT.c_str());
	}
	else {
		//ゲームオーバーテキストの表示
		DrawStringEx(OVER_TEXT_POS.x, OVER_TEXT_POS.y, TEXT_COLOR, OVER_TEXT.c_str());
	}
	SetFontSize(NORMAL_FONTSIZE);
	if (flashed_value_ > FLASHED_CENTER_VALUE) {
		//クリックテキストの表示
		DrawStringEx(CLICK_TEXT_POS.x, CLICK_TEXT_POS.y, TEXT_COLOR, CLICK_TEXT.c_str());
	}
}
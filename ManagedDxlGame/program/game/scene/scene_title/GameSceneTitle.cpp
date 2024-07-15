#include "../../../dxlib_ext/dxlib_ext.h"
#include "GameSceneTitle.h"
#include "../scene_play/GameScenePlay.h"
#include "../../gm_mgr/GameManager.h"
#include "../../gm_mgr/ResourceManager.h"

SceneTitle::SceneTitle() {
	SetBackgroundColor(32, 32, 32);

	//画像を読み込む
	back_ground_gfx_ = ResourceManager::GetInstance()->loadGraph("graphics/title_back_ground.png");
	dark_text_box_gfx_ = ResourceManager::GetInstance()->loadGraph("graphics/gray1.jpg");
	choiced_text_box_gfx_ = ResourceManager::GetInstance()->loadGraph("graphics/gray3.jpg");
	play_rule_gfx_ = ResourceManager::GetInstance()->loadGraph("graphics/play_rule.png");
	//音楽を読み込む
	bgm_snd_ = ResourceManager::GetInstance()->loadSound("sound/bgm/menu_bgm.mp3");
	pray_rule_snd_ = ResourceManager::GetInstance()->loadSound("sound/se/pray_rule_se.mp3");
	enter_snd_ = ResourceManager::GetInstance()->loadSound("sound/se/enter_se.mp3");

	//ボタンは選択されていない画像に変えておく
	start_box_gfx_ = dark_text_box_gfx_;
	pray_rule_box_gfx_ = dark_text_box_gfx_;
	close_box_gfx_ = dark_text_box_gfx_;
	
	//bgmを流す
	PlaySoundMem(bgm_snd_, DX_PLAYTYPE_LOOP);
}

SceneTitle::~SceneTitle() {
	StopSoundMem(bgm_snd_);
}

void SceneTitle::update(float delta_time) {
	mouse_pos_.x = static_cast<int>(tnl::Input::GetMousePosition().x);
	mouse_pos_.y = static_cast<int>(tnl::Input::GetMousePosition().y);
	//シーケンスの更新処理
	sequence_.update(delta_time);
}

bool SceneTitle::seqHome(float delta_time) {
	if (sequence_.isStart()) {
		//draw関数用のHOME画面のフラグを立てる
		home_flag_ = true;
		//draw関数用の遊び方のフラグをおろす
		play_rule_flag_ = false;
		//draw関数用の選択画面のフラグをおろす
		choicing_flag_ = false;
		//経過時間をリセット
		elapsed_ = 0;
	}
	//時間経過
	elapsed_ += delta_time;
	//点滅用の計算
	flashed_value_ = sin(elapsed_ * 5);

	//スタートボタンの処理
	UpdateButtonGraphics(START_TEXT_POS,START_STR, start_box_gfx_,
		enter_snd_, ChangeState::CHOICING);
	//遊び方ボタンの処理
	UpdateButtonGraphics(PLAY_RULE_TEXT_POS, PLAY_RULE_STR, pray_rule_box_gfx_,
		pray_rule_snd_, ChangeState::PLAY_RULE);

	return true;
}

bool SceneTitle::seqPlayRule(float delta_time) {
	if (sequence_.isStart()) {
		//draw関数用に遊び方のフラグを立てる
		play_rule_flag_ = true;
		//「左クリックを押してネ」を非表示にする
		flashed_value_ = 0;
	}
	//閉じるボタンの処理
	UpdateButtonGraphics(PLAY_RULE_CLOSE_TEXT_POS, CLOSE_STR, close_box_gfx_,
		pray_rule_snd_, ChangeState::HOME);

	return true;
}

bool SceneTitle::seqChoicing(float delta_time) {
	if (sequence_.isStart()) {
		//draw関数用に選択画面のフラグを立てる
		choicing_flag_ = true;
		//draw関数用のHOME画面のフラグをおろす
		home_flag_ = false;
		//「左クリックを押してネ」を非表示にする
		flashed_value_ = 0;
	}
	//簡単ボタンの処理
	UpdateButtonGraphics(EASY_TEXT_POS, EASY_STR, easy_box_gfx_,
		enter_snd_, ChangeState::EASY_GAME);
	//難しいボタンの処理
	UpdateButtonGraphics(HARD_TEXT_POS, HARD_STR, hard_box_gfx_,
		enter_snd_, ChangeState::HARD_GAME);
	//閉じるボタンの処理
	UpdateButtonGraphics(CHOICING_CLOSE_TEXT_POS, CLOSE_STR, close_box_gfx_,
		pray_rule_snd_, ChangeState::HOME);

	return true;
}

void SceneTitle::draw() {
	//背景画像の描画
	DrawGraphF(0,0, back_ground_gfx_,false);

	//透明度を上げる
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, TITLE_BACK_GFX_ALPHA);
	//タイトルの裏の黒い背景
	DrawExtendGraph(TITLE_BACK_POS_FIRST.x, TITLE_BACK_POS_FIRST.y,
		TITLE_BACK_POS_LAST.x, TITLE_BACK_POS_LAST.y, dark_text_box_gfx_, false);
	//遊び方を見ている時の後ろの背景
	if (play_rule_flag_) {
		DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, dark_text_box_gfx_, false);
	}
	//透明度を戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	//タイトルの表示
	SetFontSize(RESULT_FONTSIZE);
	DrawStringEx(TITLE_TEXT_POS.x, TITLE_TEXT_POS.y, TEXT_COLOR, TITLE_STR.c_str());
	
	//クリックテキストの表示
	SetFontSize(CLICK_FONTSIZE);
	if (flashed_value_ > 0) {
		DrawStringEx(CLICK_TEXT_POS.x, CLICK_TEXT_POS.y, 0, CLICK_STR.c_str());
	}

	SetFontSize(NORMAL_FONTSIZE);
	if (home_flag_) {
		//「スタート」のテキストボックス
		CreateTextBox(START_TEXT_POS, START_STR, start_box_gfx_);
		//「遊び方」のテキストボックス
		CreateTextBox(PLAY_RULE_TEXT_POS, PLAY_RULE_STR, pray_rule_box_gfx_);
	}
	//遊び方の表示
	if (play_rule_flag_) {
		//遊び方の画像の表示
		DrawGraphF(0, 0, play_rule_gfx_, true);
		//「閉じる」のテキストボックス
		CreateTextBox(PLAY_RULE_CLOSE_TEXT_POS, CLOSE_STR, close_box_gfx_);
	}
	//選択画面の表示
	if (choicing_flag_) {
		//「簡単」のテキストボックス
		CreateTextBox(EASY_TEXT_POS, EASY_STR, easy_box_gfx_);
		//「難しい」のテキストボックス
		CreateTextBox(HARD_TEXT_POS, HARD_STR, hard_box_gfx_);
		//「閉じる」のテキストボックス
		CreateTextBox(CHOICING_CLOSE_TEXT_POS, CLOSE_STR, close_box_gfx_);
	}
}

void SceneTitle::UpdateButtonGraphics(tnl::Vector2i text_pos, std::string str,
	int& box_gfx, int snd, ChangeState seq) {
	//マウスがスタートボタンに来ていた時
	if (IsMouseInTextBox(TextBoxPosFirst(text_pos), TextBoxPosLast(text_pos, str))) {
		//選択中の画像にテキストボックスを変える
		box_gfx = choiced_text_box_gfx_;
		//左クリックされた時
		if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_LEFT)) {
			//効果音を流す
			PlaySoundMem(snd, DX_PLAYTYPE_BACK);

			//選択画面に遷移する時
			if (seq == ChangeState::CHOICING) {
				//選択画面に遷移
				sequence_.change(&SceneTitle::seqChoicing);
			}
			//遊び方画面に遷移する時
			else if (seq == ChangeState::PLAY_RULE) {
				//遊び方画面に遷移
				sequence_.change(&SceneTitle::seqPlayRule);
			}
			//HOME画面に遷移する時
			else if (seq == ChangeState::HOME) {
				//遊び方画面に遷移
				sequence_.change(&SceneTitle::seqHome);
			}
			//ゲーム画面に遷移する時
			else {
				//簡単モードの時
				if (seq == ChangeState::EASY_GAME) {
					//ゲーム画面に遷移
					auto mgr = GameManager::GetInstance();
					mgr->changeScene(std::make_shared<ScenePlay>(false));
				}
				//難しいモードの時
				else if (seq == ChangeState::HARD_GAME) {
					//ゲーム画面に遷移
					auto mgr = GameManager::GetInstance();
					mgr->changeScene(std::make_shared<ScenePlay>(true));
				}
			}
		}
	}
	else {
		//マウスが来ていない時は暗くしておく
		box_gfx = dark_text_box_gfx_;
	}
}

bool SceneTitle::IsMouseInTextBox(
	tnl::Vector2i text_box_pos_first, tnl::Vector2i text_box_pos_last) {
	//マウスがテキストボックスの中に来たら
	if (mouse_pos_.x >= text_box_pos_first.x && mouse_pos_.x <= text_box_pos_last.x &&
		mouse_pos_.y >= text_box_pos_first.y && mouse_pos_.y <= text_box_pos_last.y) {
		return true;
	}
	else {
		return false;
	}
}

void SceneTitle::CreateTextBox(tnl::Vector2i text_pos, std::string str, int box_gfx) {
	tnl::Vector2i start_box_pos_first = TextBoxPosFirst(text_pos);
	tnl::Vector2i start_box_pos_last = TextBoxPosLast(text_pos, str);
	//テキストボックスの描画
	DrawExtendGraph(start_box_pos_first.x, start_box_pos_first.y,
		start_box_pos_last.x, start_box_pos_last.y, box_gfx, false);
	//文字の描画
	DrawStringEx(text_pos.x, text_pos.y, TEXT_COLOR, str.c_str());
}

tnl::Vector2i SceneTitle::TextBoxPosFirst(tnl::Vector2i text_pos) {
	//テキストボックスの左上の座標
	return { text_pos.x - TEXT_AROUND_WIDTH,text_pos.y - TEXT_AROUND_WIDTH };
}

tnl::Vector2i SceneTitle::TextBoxPosLast(tnl::Vector2i text_pos, std::string str) {
	return { text_pos.x + (NORMAL_FONTSIZE * static_cast<int>(str.length()) / 2) + 
		TEXT_AROUND_WIDTH, text_pos.y + NORMAL_FONTSIZE + TEXT_AROUND_WIDTH };
}
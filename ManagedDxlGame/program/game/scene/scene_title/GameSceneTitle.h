#pragma once
#include "../../gm_mgr/SceneBase.h"

class SceneTitle : public SceneBase {
public:
	SceneTitle();
	~SceneTitle();

	void update(float delta_time) override;
	void draw() override;
private:
	//現在の状態
	tnl::Sequence<SceneTitle> sequence_ = tnl::Sequence<SceneTitle>(this, &SceneTitle::seqHome);
	//HOME画面
	bool seqHome(float delta_time);
	//遊び方の画面
	bool seqPlayRule(float delta_time);
	//選択画面
	bool seqChoicing(float delta_time);
	//どの状態に移動するか
	enum ChangeState {
		HOME,
		PLAY_RULE,
		CHOICING,
		EASY_GAME,
		HARD_GAME
	};

	//黒い背景のテキストボックスを作る関数
	void CreateTextBox(tnl::Vector2i text_pos, std::string str, int box_gfx);
	//ボタンがマウスに来た時の処理をまとめた関数
	void UpdateButtonGraphics(tnl::Vector2i text_pos, std::string str, int& box_gfx, int snd, ChangeState seq);
	//マウスがテキストボックスに来たかどうか
	bool IsMouseInTextBox(tnl::Vector2i text_box_pos_first,tnl::Vector2i text_box_pos_last );
	//テキストボックスの左上の座標
	tnl::Vector2i TextBoxPosFirst(tnl::Vector2i text_pos);
	//テキストボックスの右下の座標
	tnl::Vector2i TextBoxPosLast(tnl::Vector2i text_pos, std::string str);

	//点滅の中央値(sin値で点滅させるため0)
	const float FLASHED_CENTER_VALUE = 0.0f;
	//タイトルのフォントの大きさ
	const int RESULT_FONTSIZE = 150;
	//通常フォントの大きさ
	const int NORMAL_FONTSIZE = 50;
	//クリックフォントの大きさ
	const int CLICK_FONTSIZE = 40;
	//テキストのカラーコード
	const int TEXT_COLOR = 0xffd700;
	//クリックテキストのカラーコード
	const int CLICK_TEXT_COLOR = 0;
	//タイトルの黒い背景の画像の透明度
	const int TITLE_BACK_GFX_TRANSPARENCY = 50;
	//透明度が最大の値
	const int TRANSPARENCY_MAX = 255;
	//テキストの周りの黒背景の幅
	const int TEXT_AROUND_WIDTH = 5;
	//音量の大きさ
	const int SND_VOL = 8000;
	//点滅時間の値
	const int FLASHED_VALUE = 5;
	//全角文字が2つ分の幅を取るため調整する倍率
	const int FULL_WIDTH_CHAR_RATIO = 2;
	//タイトルの文字
	const std::string TITLE_TEXT = "戦車でBANG!!!";
	//クリックの文字
	const std::string CLICK_TEXT = "左クリックを押してネ";
	//スタートの文字
	const std::string START_TEXT = "スタート";
	//遊び方の文字
	const std::string PLAY_RULE_TEXT = "遊び方";
	//閉じるの文字
	const std::string CLOSE_TEXT = "戻る";
	//閉じるの文字
	const std::string EASY_TEXT = "簡単";
	//閉じるの文字
	const std::string HARD_TEXT = "難しい";

	//背景の表示座標
	const tnl::Vector3 BACK_GROUND_POS = { 0.0f,0.0f,0.0f };
	//タイトルのテキストの座標
	const tnl::Vector2i TITLE_TEXT_POS = { 150,50 };
	//スタートのテキストの座標
	const tnl::Vector2i START_TEXT_POS = { DXE_WINDOW_WIDTH / 2 - (50 * 2),470 };
	//遊び方のテキストの座標
	const tnl::Vector2i PLAY_RULE_TEXT_POS = { DXE_WINDOW_WIDTH / 2 - 75,555 };
	//遊び方画面の閉じるのテキストの座標
	const tnl::Vector2i PLAY_RULE_CLOSE_TEXT_POS = { 970,120 };
	//選択画面の閉じるのテキストの座標
	const tnl::Vector2i CHOICING_CLOSE_TEXT_POS = { 1050,400 };
	//簡単のテキストの座標
	const tnl::Vector2i EASY_TEXT_POS = { 440,510 };
	//難しいのテキストの座標
	const tnl::Vector2i HARD_TEXT_POS = { 740,510 };
	//クリックのテキストの座標
	const tnl::Vector2i CLICK_TEXT_POS = { DXE_WINDOW_WIDTH / 2 - (CLICK_FONTSIZE * 5),640 };
	//タイトルの黒背景の位置の左上の位置
	const tnl::Vector2i TITLE_BACK_POS_FIRST = { 140,30 };
	//タイトルの黒背景の位置の右下の位置
	const tnl::Vector2i TITLE_BACK_POS_LAST = { DXE_WINDOW_WIDTH - 140,210 };
	
	//マウスの座標
	tnl::Vector2i mouse_pos_;
	//背景の画像パス
	int back_ground_gfx_;
	//後ろの黒い背景の画像パス
	int dark_text_box_gfx_;
	//選択されてる黒い背景の画像パス
	int choiced_text_box_gfx_;
	//遊び方の画像パス
	int play_rule_gfx_;
	//遊び方の選択音の音楽パス
	int pray_rule_snd_;
	//決定音の音楽パス
	int enter_snd_;
	//スタートボタンにマウスが来ている時の表示画像
	int start_box_gfx_;
	//遊び方ボタンにマウスが来ている時の表示画像
	int pray_rule_box_gfx_;
	//閉じるボタンにマウスが来ている時の表示画像
	int close_box_gfx_;
	//簡単ボタンにマウスが来ている時の表示画像
	int easy_box_gfx_;
	//難しいボタンにマウスが来ている時の表示画像
	int hard_box_gfx_;
	//経過時間
	float elapsed_ = 0;
	//点滅の値
	float flashed_value_ = 0;
	//HOME画面を表示するフラグ
	bool home_flag_ = false;
	//遊び方を表示するフラグ
	bool play_rule_flag_ = false;
	//選択画面を表示するフラグ
	bool choicing_flag_ = false;
};
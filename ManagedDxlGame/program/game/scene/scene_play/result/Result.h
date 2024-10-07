#pragma once

class SceneResult{
public:
	SceneResult(bool cleared = false);
	~SceneResult();

	void update(float delta_time);
	void draw();

private:
	//結果のフォントの大きさ
	const int RESULT_FONTSIZE = 150;
	//通常フォントの大きさ
	const int NORMAL_FONTSIZE = 50;

	//クリアの文字
	const std::string CLEAR_TEXT = "ゲームクリア";
	//ゲームオーバーの文字
	const std::string OVER_TEXT = "ゲームオーバー";
	//クリックの文字
	const std::string CLICK_TEXT = "左クリックを押してネ";


	//背景の表示座標
	const tnl::Vector3 BACK_GROUND_POS = { 0.0f,0.0f,0.0f };
	//クリアテキストの座標
	const tnl::Vector2i CLEAR_TEXT_POS = { 180,DXE_WINDOW_HEIGHT / 2 - RESULT_FONTSIZE / 2 - 50 };
	//ゲームオーバーテキストの座標
	const tnl::Vector2i OVER_TEXT_POS = { 100,DXE_WINDOW_HEIGHT / 2 - RESULT_FONTSIZE / 2 - 50 };
	//エンターの座標
	const tnl::Vector2i CLICK_TEXT_POS = { 345,460 };
	//点滅の中央値(sin値で点滅させるため0)
	const float FLASHED_CENTER_VALUE = 0.0f;
	//テキストのカラーコード
	const int TEXT_COLOR = 0xffd700;
	//黒い背景の画像の透明度
	const int BACK_GFX_ALPHA = 125;
	//点滅時間の値
	const int FLASHED_VALUE = 5;
	//透明度が最大の値
	const int TRANSPARENCY_MAX = 255;

	//経過時間
	float timeleft_ = 0;
	//点滅の値
	float flashed_value_ = 0;
	//ゲームがクリアされたかどうか
	bool cleared_ = false;

	//後ろの黒い背景の画像パス
	int back_gfx_;
	//ゲーム終了時の効果音
	int finish_snd_;
	//決定音の音楽パス
	int enter_snd_;
};
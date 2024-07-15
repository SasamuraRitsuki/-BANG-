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
	//クリアテキストの座標
	const tnl::Vector2i CLEAR_TEXT_POS = { 180,DXE_WINDOW_HEIGHT / 2 - RESULT_FONTSIZE / 2 - 50 };
	//ゲームオーバーテキストの座標
	const tnl::Vector2i OVER_TEXT_POS = { 100,DXE_WINDOW_HEIGHT / 2 - RESULT_FONTSIZE / 2 - 50 };
	//エンターの座標
	const tnl::Vector2i CLICK_TEXT_POS = { 345,460 };
	//テキストのカラーコード
	const int TEXT_COLOR = 0xffd700;
	//黒い背景の画像の透明度
	const int BACK_GFX_ALPHA = 125;

	//経過時間
	float timeleft_ = 0;
	//点滅の値
	float elapsed_ = 0;
	//ゲームがクリアされたかどうか
	bool cleared_ = false;

	//後ろの黒い背景の画像パス
	int back_gfx_;
	//ゲーム終了時の効果音
	int finish_snd_;
	//決定音の音楽パス
	int enter_snd_;
};
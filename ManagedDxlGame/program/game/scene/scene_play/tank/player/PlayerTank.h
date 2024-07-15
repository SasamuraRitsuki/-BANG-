#pragma once
#include "../TankBase.h"

class Bullet;

struct MoveFloorState {
	// 動く床に乗っているかどうかを示すフラグ
	bool is_riding_;               
	// 動く床に乗り始めた最初のフレームかどうかを示すフラグ
	bool just_started_riding_;
	// 動く床に乗る前の戦車の位置
	float prev_tank_pos_x_;
	// 動く床に乗る前の床の位置
	float prev_delta_move_floor_distance_;

	// コンストラクタで初期値を設定
	MoveFloorState()
		: is_riding_(false), just_started_riding_(true), prev_tank_pos_x_(0.0f), prev_delta_move_floor_distance_(0.0f) {}
};

class PlayerTank : public TankBase{
public:
	PlayerTank();
	~PlayerTank();

	void update(float delta_time) override;
	void draw(Shared<dxe::Camera> camera) override;

	//敵と弾の当たり判定
	bool TryEnemyCollision(std::shared_ptr<Bullet> blt, tnl::Vector3 enemy_pos, tnl::Vector3 enemy_size);
	//着地時の処理
	void LandingOnTheFloor(); 

	//セッター
	void setIsRidingMoveFloor1(bool is_riding);
	void setIsRidingMoveFloor2(bool is_riding);

private:
	//現在の状態
	tnl::Sequence<PlayerTank> sequence_ = tnl::Sequence<PlayerTank>(this, &PlayerTank::seqNormal);
	//通常時
	bool seqNormal(float delta_time);
	//死亡中
	bool seqDead(float delta_time);

	//向きを変える
	void DirChange();
	//マウスへの角度計算
	void AngleCalcToMouse();
	//回転する
	void TankRolling(float delta_time);
	//加減速計算
	float EaseInOut(float t);
	//動く床の処理計算
	void MoveFloorCalc(MoveFloorState& floor_state, float& tank_pos, float delta_move_floor_pos,float move_vel);

	// 動く床1に乗っている状態
	MoveFloorState move_floor1_state_;
	// 動く床2に乗っている状態
	MoveFloorState move_floor2_state_;


	//スタート位置
	const tnl::Vector3 FIRST_POS = { 0,50,0 };
	//tankの形
	const tnl::Vector3 TANK_FORM = { 1.0f, 0.75f, 1.0f };
	//弾の画像の位置と大きさ
	const tnl::Vector3 BULLET_POS = { 32, 50, 0.8f };
	//HPのゲージ位置
	const tnl::Vector2i HP_GAUGE_POS = { DXE_WINDOW_WIDTH/3 ,DXE_WINDOW_HEIGHT * 4 / 5 };
	//スピンゲージ位置
	const tnl::Vector2i SPIN_GAUGE_POS = { HP_GAUGE_POS.x ,576 + 60 };
	//テキストの位置
	const tnl::Vector2i HP_TEXT_POS = { HP_GAUGE_POS.x - 90, HP_GAUGE_POS.y };
	const tnl::Vector2i SPIN_TEXT_POS = { SPIN_GAUGE_POS.x - 150, SPIN_GAUGE_POS.y - 5 };
	const tnl::Vector2i SPIN_CLICK_TEXT_POS = { SPIN_TEXT_POS.x + 600, SPIN_TEXT_POS.y };
	//下の白い背景の位置の左上の位置
	const tnl::Vector2i DOWN_BACK_POS_FIRST = { 140,HP_GAUGE_POS.y - 20 };
	//下の白い背景の位置の右下の位置
	const tnl::Vector2i DOWN_BACK_POS_LAST = { DXE_WINDOW_WIDTH - 140,SPIN_GAUGE_POS.y + 60 };
	//上の白い背景の位置の左上の位置
	const tnl::Vector2i UP_BACK_POS_FIRST = { 0,0 };
	//上の白い背景の位置の右下の位置
	const tnl::Vector2i UP_BACK_POS_LAST = { 64 * 5,104 };
	//移動量
	const float MOVE_SPEED = 5;
	//tankの大きさ
	const float TANK_SIZE = 80;
	//ジャンプ力
	const float JUMP_VALUE = 10;
	//重力の初期値
	const float GRAVITY_VALUE = 0.49f;
	//回転時間
	const float ROLLING_TIME = 0.8f;
	//回転可能時間
	const float ROTATABLE_TIME = ROLLING_TIME * 2;
	//hpが減りきる時間
	const float HP_ELAPSED_LIMIT = 1;
	//初期HP
	const int TANK_HP = 3;
	//落下死亡ライン
	const int DEAD_Y = -500;
	//弾の発射可能数
	const int BULLET_MAX = 5;
	//回転速度
	const int ROLLING_SPEED = 25;
	//テキストのカラーコード
	const int TEXT_COLOR = 0;

	//マウスの座標
	tnl::Vector3 mouse_pos_;
	//重力による垂直方向の大きさの値
	float jump_value_ = 0;
	//回転の経過時間
	float rolling_elapsed_ = 0;
	//HPバーの右端
	float hp_bar_right_ = static_cast<float>(HP_GAUGE_POS.x + HP_GAUGE_POS.x);
	//スピンバーの右端
	float spin_bar_right_ = static_cast<float>(SPIN_GAUGE_POS.x + HP_GAUGE_POS.x);
	//回転の速さ
	float rolling_speed_ = 0;
	//hpバーの経過時間
	float hp_elapsed_ = 0;
	//マウスへの角度
	int angle_to_mouse_;
	//マウスでクリックしたかどうか
	bool clicked_ = false;
	//1クリックで1発撃ったか
	bool shoted_ = false;
	//弾が消える判定を受けたか
	bool is_alive_bullet_ = false;
	//落下中かどうか
	bool falling = false;
	//回転可能かどうか
	bool rotatable_ = false;
	//hp計算中
	bool hp_calc_ = false;
	//hpが0になった時に動きを止める
	bool move_stop_ = false;

	//-----------------------画像や音楽パス-------------------------------
	//スピンゲージの画像パス
	int spin_gfx_;
	//後ろの白い背景の画像パス
	int back_gfx_;
	//弾の画像パス
	int bullet_gfx_;
	//回転の音楽パス
	int rolling_snd_;
	//ジャンプの音楽パス
	int jump_snd_;
};
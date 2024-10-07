#pragma once
#include "../../TankBase.h"

class Bullet;

class EnemyTank : public TankBase{
public:
	EnemyTank(const tnl::Vector3& spawn_pos, const float& dir_angle, bool hard_mode);
	~EnemyTank();

	void update(float delta_time) override;
	void draw(Shared<dxe::Camera> camera) override;

private:
	//現在の状態
	tnl::Sequence<EnemyTank> sequence_state_ = tnl::Sequence<EnemyTank>(this, &EnemyTank::seqNormal);
	//現在の弾を撃っているかどうかの状態
	tnl::Sequence<EnemyTank> sequence_shooting_ = tnl::Sequence<EnemyTank>(this, &EnemyTank::seqNotShooting);
	//「現在の状態」専用のシーケンス
	//通常時
	bool seqNormal(float delta_time);
	//ダメージ中
	bool seqDamaged(float delta_time);
	//死亡中
	bool seqDead(float delta_time);
	//「現在の弾を撃っているかどうかの状態」専用のシーケンス
	//弾を撃っていない
	bool seqNotShooting(float delta_time);
	//弾の発射中
	bool seqShooting(float delta_time);

	//生存中の更新処理
	void UpdateAliveTank(float delta_time);
	//ランダムで弾を撃つ間隔を抽選する
	void RandomShotCount();
	//ランダムで向きを変える
	void RandomDirChange();
	//戦車への角度計算
	void AngleCalcToTank();
	//Hpの計算
	void HpCalc();
	//弾との当たり判定
	bool TryBulletCollision(std::shared_ptr<Bullet> blt);

	//tankの形
	const tnl::Vector3 TANK_FORM = { 1.0f, 0.75f, 1.0f };
	//tankの大きさ
	const float TANK_SIZE = 80.0f;
	//重力の初期値
	const float GRAVITY_VALUE = 0.49f;
	//hpが減りきる時間
	const float HP_ELAPSED_LIMIT = 0.25f;
	//移動量
	const float MOVE_SPEED = 3.0f;
	//弾を撃ってから動き出すまでの秒数
	const float SHOT_INTERVAL = 1.0f;
	//HPバーの幅(横、縦)
	const tnl::Vector2i BAR_WIDTH = { 60, 10 };
	//HPバーの半分の長さ
	const float HP_BAR_HALF = BAR_WIDTH.x / 2.0f;
	//HPバーが戦車からどのくらいの高さにあるか
	const float HP_BAR_HEIGHT = 100.0f;
	//難しいモードの弾の撃つ間隔の平均値
	const float SHOT_COUNT_AVG_HARD = 4.0f;
	//簡単モードの弾の撃つ間隔の平均値
	const float SHOT_COUNT_AVG_EASY = 6.0f;
	//初期HP
	const int TANK_HP = 3;
	//ジャンプ力
	const int JUMP_VALUE = 10;
	//落下死亡ライン
	const int DEAD_Y = -500;
	//弾の発射可能数
	const int BULLET_MAX = 10;
	//ランダムで向きを変える秒数
	const int DIR_CHANGE_COUNT = 1;
	//角度のオフセットを示す定数
	const int ANGLE_OFFSET = 180;

	//-----------弾を撃つ間隔のランダム調整に使う定数--------------
	//弾の乱数の範囲（±10の幅）
	const int RANDOM_SHOT_RANGE = 20;
	//弾の乱数のオフセット値（中央値）
	const int RANDOM_SHOT_OFFSET = 10 - 1;
	//弾の乱数のスケール（-0.9から+1.0）
	const float RANDOM_SHOT_SCALE = 0.1f;
	
	//向きの乱数の範囲（±2の幅）
	const int RANDOM_DIR_RANGE = 5;
	//向きの乱数のオフセット値（中央値）
	const int RANDOM_DIR_OFFSET = 2;
	//向きの乱数のスケール（-1.0から+1.0）
	const float RANDOM_DIR_POWER = 0.5f;

	//進むか止まるかの乱数の範囲
	const int RANDOM_GO_RANGE = 2;
	//-------------------------------------------------------------
	//戦車への距離
	tnl::Vector2i distance_to_tank_;
	//画面上のhpバーの座標
	tnl::Vector3 screen_hp_bar_pos_;
	//弾を発射するまでの秒数
	float shot_elapsed_ = 0.0f;
	//ランダムで動きを変えるまでの秒数
	float random_elapsed_ = 0.0f;
	//HPバーの右端
	float hp_bar_right_ = screen_hp_bar_pos_.x + static_cast<float>(BAR_WIDTH.x);
	//hpバーの経過時間
	float hp_elapsed_ = 0.0f;
	//hpバーの長さ
	float hp_bar_length_ = static_cast<float>(BAR_WIDTH.x);
	//hpバーの右端の始動位置
	float hp_bar_start_right = static_cast<float>(BAR_WIDTH.x);
	//弾を撃つまでの秒数(ランダムに変わる)
	float shot_count_ = 0.0f;
	//向く角度の値
	float dir_value_ = 0.0f;
	//向いている角度
	float dir_angle_ = 0.0f;
	//戦車への角度
	int angle_to_tank_ = 0;
	//進むか止まるか
	bool go_judge_ = false;
	//hpのカウント中
	bool is_hp_counting_ = false;
	//難しいモードかどうか
	bool hard_mode_ = false;

	//-----------------------音楽パス-------------------------------
	//やられた時の音楽パス
	int dead_snd_;
	//反射の音楽パス
	int reflection_snd_;
};
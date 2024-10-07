#pragma once
#include "../../gm_mgr/SceneBase.h"

class EnemyTank;
class PlayerTank;
class Floor;
class Wall;
class SceneResult;
class Spawner;
class SkyBox;

class ScenePlay : public SceneBase {
public:
	ScenePlay(bool hard_mode);
	~ScenePlay();

	void update(float delta_time) override;
	void draw() override;

	//当たり判定をまとめたもの
	void CollisionSet();
	//当たり判定をまとめたもの(敵用)
	void CollisionSetEnemy(std::shared_ptr<EnemyTank> enemy);
	//床との当たり判定
	void FloorCollision(int move_floor, tnl::Vector3 floorpos, tnl::Vector3 floorsize);
	//壁との当たり判定(自機)
	void TankWallCollision(tnl::Vector3& tank_pos, tnl::Vector3 tank_size, tnl::Vector3 wall_pos, tnl::Vector3 wall_size);

private:
	//現在のゲームの状態
	tnl::Sequence<ScenePlay> sequence_ = tnl::Sequence<ScenePlay>(this, &ScenePlay::seqInGame);
	//ゲームプレイ中
	bool seqInGame(float delta_time);
	//リザルト画面
	bool seqResult(float delta_time);

	//カメラのポインタ
	Shared<dxe::Camera> camera_;
	//スカイボックスのポインタ
	Shared<SkyBox> skybox_;
	//自機のポインタ
	Shared<PlayerTank> player_ = nullptr;
	//壁のポインタ
	Shared<Wall> wall_ = nullptr;
	//床のポインタ
	Shared<Floor> floor_ = nullptr;
	//リザルトのポインタ
	Shared<SceneResult>result_;
	//スポナーのポインタ
	Shared<Spawner> spawner_;

	//床の種類
	const enum FLOOR_SORT {
		STOP_FLOOR,
		MOVE_FLOOR1,
		MOVE_FLOOR2
	};
	//カメラの座標の初期値
	const tnl::Vector3 FIRST_CAMERA_POS = { 0.0f, 650.0f, -500.0f };
	//カメラ追従の速度の倍率
	const float CAMERA_FOLLOW_SPEED = 0.1f;
	//衝突判定の補正後の間
	const float POST_COLLISION_GAP = 0.1f;
	//カメラの追従する座標の高さの値(同じ高さから俯瞰したいため)
	const float CAMERA_FOCUS_POS_Y = 0.0f;
};
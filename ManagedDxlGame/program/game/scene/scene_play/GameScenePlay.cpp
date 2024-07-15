#include "../../../dxlib_ext/dxlib_ext.h"
#include "GameScenePlay.h"
#include "../../gm_mgr/GameManager.h"
#include "../../gm_mgr/ResourceManager.h"
#include "tank/enemy_spawner/Spawner.h"
#include "tank/player/PlayerTank.h"
#include "tank/enemy_spawner/enemy/EnemyTank.h"
#include "gm_obj/Floor.h"
#include "gm_obj/Wall.h"
#include "result/Result.h"

ScenePlay::ScenePlay(bool hard_mode) {
	ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f)); 
	SetBackgroundColor(50, 50, 50);
	player_ = std::make_shared<PlayerTank>(); 
	floor_ = std::make_shared<Floor>(hard_mode);
	wall_ = std::make_shared<Wall>();
	spawner_ = std::make_shared<Spawner>(hard_mode);

	//自機に必要なポインタを渡す
	player_->setFloorPointer(floor_);
	player_->setSpawnerPointer(spawner_);
	player_->setWallPointer(wall_);
	//敵のリストに必要なポインタを渡す
	for (auto enemy : spawner_->enemy_) {
		enemy->setPlayerPointer(player_);
		enemy->setWallPointer(wall_);
	}

	camera_ = std::make_shared<dxe::Camera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);

	camera_->pos_ = FIRST_CAMERA_POS;

	//後ろの背景のスカイボックスの設定
	SetDefaultLightParameter("directional_light_parameter.bin");
	skybox_ = dxe::Mesh::CreateCubeMV(30000, 20, 20);
	//難しいモードの時のスカイボックス
	if (hard_mode) {
		skybox_->setTexture(dxe::Texture::CreateFromFile("graphics/skybox/_skybox_a.png"));
	}
	//簡単モードの時のスカイボックス
	else {
		skybox_->setTexture(dxe::Texture::CreateFromFile("graphics/skybox/_skybox_c.png"));
	}
	skybox_->loadMaterial("material.bin");
	skybox_->rot_ = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(90));

	//音楽を読み込む
	bgm_snd_ = ResourceManager::GetInstance()->loadSound("sound/bgm/battle_bgm.mp3");

	//bgmの生成
	PlaySoundMem(bgm_snd_, DX_PLAYTYPE_LOOP);
}

ScenePlay::~ScenePlay() {

}

void ScenePlay::update(float delta_time) {
	
	sequence_.update(delta_time);

}

bool ScenePlay::seqInGame(float delta_time) {
	//自機の更新処理
	player_->update(delta_time);
	//自機の更新処理
	spawner_->update(delta_time);
	//自機の更新処理
	floor_->update(delta_time);
	//スカイボックスをちょっとずつ動かす
	skybox_->rot_ *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(0.06f));

	//当たり判定の処理
	CollisionSet();
	for (auto enemy : spawner_->enemy_) {
		CollisionSetEnemy(enemy);
	}

	//移動した時のカメラ位置
	tnl::Vector3 NEXT_CAMERA_POS = { player_->getTankPos().x - FIRST_CAMERA_POS.x,FIRST_CAMERA_POS.y,player_->getTankPos().z + FIRST_CAMERA_POS.z };

	//カメラ追従
	camera_->pos_ -= (camera_->pos_ - NEXT_CAMERA_POS) * 0.1f;
	camera_->target_ -= (camera_->target_ - tnl::Vector3{ player_->getTankPos().x, 0,player_->getTankPos().z }) * 0.1f;
	//カメラの更新処理
	camera_->update();

	if (player_->getTankDead() ||		//自機が死んだら
		spawner_->getTankCleared()) {	//敵を殲滅したら
		//seqResultに飛ぶ
		sequence_.change(&ScenePlay::seqResult);
	}
	return true;
}

bool ScenePlay::seqResult(float delta_time) {

	if (sequence_.isStart()) {
		//bgmのストップ
		StopSoundMem(bgm_snd_);
		//結果画面の生成
		result_ = std::make_shared<SceneResult>(spawner_->getTankCleared());
	}

	//結果画面のupdate
	result_->update(delta_time);

	return true;
}

void ScenePlay::draw() {
	//スカイボックスの描画
	skybox_->render(camera_);
	//壁の描画
	wall_->draw(camera_);
	//床の描画
	floor_->draw(camera_);
	//スポナーの描画
	spawner_->draw(camera_);
	//自機の描画
	player_->draw(camera_);
	//リザルトの描画
	if (result_ != nullptr) {
		result_->draw();
	}
}

void ScenePlay::CollisionSet() {

	FloorCollision(FLOOR_SORT::STOP_FLOOR, floor_->getFloor1Pos(), floor_->getFloor1Size());
	FloorCollision(FLOOR_SORT::STOP_FLOOR, floor_->getFloor2Pos(), floor_->getFloor2Size());
	FloorCollision(FLOOR_SORT::MOVE_FLOOR1, floor_->getMoveFloor1Pos(), floor_->getMoveFloorSize());
	FloorCollision(FLOOR_SORT::MOVE_FLOOR2, floor_->getMoveFloor2Pos(), floor_->getMoveFloorSize());
	
	tnl::Vector3 tank_pos = player_->getTankPos();
	tnl::Vector3 tank_size = player_->getTankSize();

	TankWallCollision(tank_pos, tank_size, wall_->getWall1Pos(), wall_->getWall1Size());
	TankWallCollision(tank_pos, tank_size, wall_->getWall2Pos(), wall_->getWall2Size());
	TankWallCollision(tank_pos, tank_size, wall_->getWall3Pos(), wall_->getWall3Size());
	TankWallCollision(tank_pos, tank_size, wall_->getWall4Pos(), wall_->getWall4Size());
	TankWallCollision(tank_pos, tank_size, wall_->getWall5Pos(), wall_->getWall5Size());

	player_->setTankPos(tank_pos);
}

void ScenePlay::CollisionSetEnemy(std::shared_ptr<EnemyTank> enemy) {

	tnl::Vector3 tank_pos = enemy->getTankPos();
	tnl::Vector3 enemy_tank_size = enemy->getTankSize();

	TankWallCollision(tank_pos, enemy_tank_size, wall_->getWall1Pos(), wall_->getWall1Size());
	TankWallCollision(tank_pos, enemy_tank_size, wall_->getWall2Pos(), wall_->getWall2Size());
	TankWallCollision(tank_pos, enemy_tank_size, wall_->getWall3Pos(), wall_->getWall3Size());
	TankWallCollision(tank_pos, enemy_tank_size, wall_->getWall4Pos(), wall_->getWall4Size());
	TankWallCollision(tank_pos, enemy_tank_size, wall_->getWall5Pos(), wall_->getWall5Size());
	TankWallCollision(tank_pos, enemy_tank_size, wall_->getWallVirtualPos(), wall_->getWallVirtualSize());

	enemy->setTankPos(tank_pos);
}


void ScenePlay::FloorCollision(
	int move_floor,				//どの床に乗っているか
	tnl::Vector3 floorpos,		//床の座標
	tnl::Vector3 floorsize) {	//床の大きさのサイズ

	tnl::Vector3 tank_pos = player_->getTankPos();
	tnl::Vector3 tank_size = player_->getTankSize();

	//自機と床が衝突した時
	if (tnl::IsIntersectAABB(tank_pos, tank_size, floorpos, floorsize)) {
		//DrawStringEx(100, 400, 0, "衝突");

		//座標の補正
		tnl::eCorrResAABB n = tnl::CorrectPositionAABB(
			tank_pos,
			floorpos,
			tank_size,
			floorsize,
			tank_pos,
			floorpos,
			tnl::eCorrTypeAABB::PWRFL_B,
			tnl::eCorrTypeAABB::PWRFL_B,
			tnl::eCorrTypeAABB::PWRFL_B, 0.1f);

		//着地時の処理
		if (tnl::eCorrResAABB::UP == n) {
			//自機の座標などの処理
			player_->LandingOnTheFloor();
		
			// 動かない床に乗った時
			if (move_floor == FLOOR_SORT::STOP_FLOOR) {
				player_->setIsRidingMoveFloor1(false);
				player_->setIsRidingMoveFloor2(false);
			}
			//動く床1に乗った時
			if (move_floor == FLOOR_SORT::MOVE_FLOOR1) {
				player_->setIsRidingMoveFloor1(true);
				player_->setIsRidingMoveFloor2(false);
			}
			//動く床2に乗った時
			if (move_floor == FLOOR_SORT::MOVE_FLOOR2) {
				player_->setIsRidingMoveFloor1(false);
				player_->setIsRidingMoveFloor2(true);
			}		
		}
	}
	//自機の座標の更新
	player_->setTankPos(tank_pos);
}



void ScenePlay::TankWallCollision(
	tnl::Vector3& tank_pos,	tnl::Vector3 tank_size,	tnl::Vector3 wall_pos,	tnl::Vector3 wall_size){


	if (tnl::IsIntersectAABB(tank_pos, tank_size, wall_pos, wall_size)) {
		//DrawStringEx(100, 400, 0, "衝突");

		tnl::eCorrResAABB n = tnl::CorrectPositionAABB(
			tank_pos,
			wall_pos,
			tank_size,
			wall_size,
			tank_pos,
			wall_pos,
			tnl::eCorrTypeAABB::PWRFL_B,
			tnl::eCorrTypeAABB::PWRFL_B,
			tnl::eCorrTypeAABB::PWRFL_B,
			0.1f);
	}
}
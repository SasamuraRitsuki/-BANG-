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

	//���@�ɕK�v�ȃ|�C���^��n��
	player_->setFloorPointer(floor_);
	player_->setSpawnerPointer(spawner_);
	player_->setWallPointer(wall_);
	//�G�̃��X�g�ɕK�v�ȃ|�C���^��n��
	for (auto enemy : spawner_->enemy_) {
		enemy->setPlayerPointer(player_);
		enemy->setWallPointer(wall_);
	}

	camera_ = std::make_shared<dxe::Camera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);

	camera_->pos_ = FIRST_CAMERA_POS;

	//���̔w�i�̃X�J�C�{�b�N�X�̐ݒ�
	SetDefaultLightParameter("directional_light_parameter.bin");
	skybox_ = dxe::Mesh::CreateCubeMV(30000, 20, 20);
	//������[�h�̎��̃X�J�C�{�b�N�X
	if (hard_mode) {
		skybox_->setTexture(dxe::Texture::CreateFromFile("graphics/skybox/_skybox_a.png"));
	}
	//�ȒP���[�h�̎��̃X�J�C�{�b�N�X
	else {
		skybox_->setTexture(dxe::Texture::CreateFromFile("graphics/skybox/_skybox_c.png"));
	}
	skybox_->loadMaterial("material.bin");
	skybox_->rot_ = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(90));

	//���y��ǂݍ���
	bgm_snd_ = ResourceManager::GetInstance()->loadSound("sound/bgm/battle_bgm.mp3");

	//bgm�̐���
	PlaySoundMem(bgm_snd_, DX_PLAYTYPE_LOOP);
}

ScenePlay::~ScenePlay() {

}

void ScenePlay::update(float delta_time) {
	
	sequence_.update(delta_time);

}

bool ScenePlay::seqInGame(float delta_time) {
	//���@�̍X�V����
	player_->update(delta_time);
	//���@�̍X�V����
	spawner_->update(delta_time);
	//���@�̍X�V����
	floor_->update(delta_time);
	//�X�J�C�{�b�N�X��������Ƃ�������
	skybox_->rot_ *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(0.06f));

	//�����蔻��̏���
	CollisionSet();
	for (auto enemy : spawner_->enemy_) {
		CollisionSetEnemy(enemy);
	}

	//�ړ��������̃J�����ʒu
	tnl::Vector3 NEXT_CAMERA_POS = { player_->getTankPos().x - FIRST_CAMERA_POS.x,FIRST_CAMERA_POS.y,player_->getTankPos().z + FIRST_CAMERA_POS.z };

	//�J�����Ǐ]
	camera_->pos_ -= (camera_->pos_ - NEXT_CAMERA_POS) * 0.1f;
	camera_->target_ -= (camera_->target_ - tnl::Vector3{ player_->getTankPos().x, 0,player_->getTankPos().z }) * 0.1f;
	//�J�����̍X�V����
	camera_->update();

	if (player_->getTankDead() ||		//���@�����񂾂�
		spawner_->getTankCleared()) {	//�G��r�ł�����
		//seqResult�ɔ��
		sequence_.change(&ScenePlay::seqResult);
	}
	return true;
}

bool ScenePlay::seqResult(float delta_time) {

	if (sequence_.isStart()) {
		//bgm�̃X�g�b�v
		StopSoundMem(bgm_snd_);
		//���ʉ�ʂ̐���
		result_ = std::make_shared<SceneResult>(spawner_->getTankCleared());
	}

	//���ʉ�ʂ�update
	result_->update(delta_time);

	return true;
}

void ScenePlay::draw() {
	//�X�J�C�{�b�N�X�̕`��
	skybox_->render(camera_);
	//�ǂ̕`��
	wall_->draw(camera_);
	//���̕`��
	floor_->draw(camera_);
	//�X�|�i�[�̕`��
	spawner_->draw(camera_);
	//���@�̕`��
	player_->draw(camera_);
	//���U���g�̕`��
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
	int move_floor,				//�ǂ̏��ɏ���Ă��邩
	tnl::Vector3 floorpos,		//���̍��W
	tnl::Vector3 floorsize) {	//���̑傫���̃T�C�Y

	tnl::Vector3 tank_pos = player_->getTankPos();
	tnl::Vector3 tank_size = player_->getTankSize();

	//���@�Ə����Փ˂�����
	if (tnl::IsIntersectAABB(tank_pos, tank_size, floorpos, floorsize)) {
		//DrawStringEx(100, 400, 0, "�Փ�");

		//���W�̕␳
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

		//���n���̏���
		if (tnl::eCorrResAABB::UP == n) {
			//���@�̍��W�Ȃǂ̏���
			player_->LandingOnTheFloor();
		
			// �����Ȃ����ɏ������
			if (move_floor == FLOOR_SORT::STOP_FLOOR) {
				player_->setIsRidingMoveFloor1(false);
				player_->setIsRidingMoveFloor2(false);
			}
			//������1�ɏ������
			if (move_floor == FLOOR_SORT::MOVE_FLOOR1) {
				player_->setIsRidingMoveFloor1(true);
				player_->setIsRidingMoveFloor2(false);
			}
			//������2�ɏ������
			if (move_floor == FLOOR_SORT::MOVE_FLOOR2) {
				player_->setIsRidingMoveFloor1(false);
				player_->setIsRidingMoveFloor2(true);
			}		
		}
	}
	//���@�̍��W�̍X�V
	player_->setTankPos(tank_pos);
}



void ScenePlay::TankWallCollision(
	tnl::Vector3& tank_pos,	tnl::Vector3 tank_size,	tnl::Vector3 wall_pos,	tnl::Vector3 wall_size){


	if (tnl::IsIntersectAABB(tank_pos, tank_size, wall_pos, wall_size)) {
		//DrawStringEx(100, 400, 0, "�Փ�");

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
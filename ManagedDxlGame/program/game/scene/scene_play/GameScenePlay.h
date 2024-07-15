#pragma once
#include "../../gm_mgr/SceneBase.h"

class EnemyTank;
class PlayerTank;
class Floor;
class Wall;
class SceneResult;
class Spawner;

class ScenePlay : public SceneBase {
public:
	ScenePlay(bool hard_mode);
	~ScenePlay();

	void update(float delta_time) override;
	void draw() override;

	//�����蔻����܂Ƃ߂�����
	void CollisionSet();
	//�����蔻����܂Ƃ߂�����(�G�p)
	void CollisionSetEnemy(std::shared_ptr<EnemyTank> enemy);
	//���Ƃ̓����蔻��
	void FloorCollision(int move_floor, tnl::Vector3 floorpos, tnl::Vector3 floorsize);
	//�ǂƂ̓����蔻��(���@)
	void TankWallCollision(tnl::Vector3& tank_pos, tnl::Vector3 tank_size,tnl::Vector3 wall_pos, tnl::Vector3 wall_size);

private:
	//���݂̃Q�[���̏��
	tnl::Sequence<ScenePlay> sequence_ = tnl::Sequence<ScenePlay>(this, &ScenePlay::seqInGame);
	//�Q�[���v���C��
	bool seqInGame(float delta_time);
	//���U���g���
	bool seqResult(float delta_time);

	//�J�����̃|�C���^
	Shared<dxe::Camera> camera_;
	//�X�J�C�{�b�N�X�̃|�C���^
	Shared<dxe::Mesh> skybox_;
	//���@�̃|�C���^
	Shared<PlayerTank> player_ = nullptr;
	//�ǂ̃|�C���^
	Shared<Wall> wall_ = nullptr;
	//���̃|�C���^
	Shared<Floor> floor_ = nullptr;
	//���U���g�̃|�C���^
	Shared<SceneResult>result_;
	//�X�|�i�[�̃|�C���^
	Shared<Spawner> spawner_;

	//���̎��
	const enum FLOOR_SORT {
		STOP_FLOOR,
		MOVE_FLOOR1,
		MOVE_FLOOR2
	};
	//�J�����̏����l
	const tnl::Vector3 FIRST_CAMERA_POS = { 0, 650, -500 };
};
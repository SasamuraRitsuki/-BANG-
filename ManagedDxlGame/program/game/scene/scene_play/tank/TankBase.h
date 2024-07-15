#pragma once

class Wall;
class Floor;
class Spawner;
class Bullet;
class PlayerTank;

class TankBase {
protected:

	virtual void update(float delta_time) {};
	virtual void draw(Shared<dxe::Camera> camera) {};

	//�ǂƂ̓����蔻��
	bool TryBulletToWallCollision(std::shared_ptr<Bullet> blt, std::shared_ptr<Wall> wall);
	//���ʂ̉摜�p�X�����[�h����֐�
	void LoadCommonGraph();
	//���ʂ̉��y�p�X�����[�h����֐�
	void LoadCommonSound();

	//�d�S�~�̔��a
	const float CENTROID_RADIUS = 20;
	//����
	const float MASS = 98;
	//���C
	const float FRICTION = 1;

	//------------------�錾�̂�------------------
	//���b�V��
	Shared<dxe::Mesh> mesh_ = nullptr;
	//���@�̋��L�|�C���^
	Shared<PlayerTank> player_;
	//�X�|�i�[�̋��L�|�C���^
	Shared<Spawner> spawner_;
	//���̋��L�|�C���^
	Shared<Floor> floor_;
	//�ǂ̋��L�|�C���^
	Shared<Wall> wall_;

	//���݂�tank�̈ʒu
	tnl::Vector3 tank_pos_;
	//tank�̔���p�̑傫��
	tnl::Vector3 tank_size_;
	//�e�̔��˂̉��y�p�X
	int shot_snd_;
	//�_���[�W�̉��y�p�X(����)
	int damage_snd_;
	//HP�̉摜�p�X(����)
	int hp_gfx_;
	//�o�[�̔w�i�摜�p�X(����)
	int hp_back_gfx_;
	//�c��HP
	int tank_hp_;
	//���S����
	bool dead_ = false;
	//�_���[�W���󂯂�
	bool damaged_ = false;
	//��]��
	bool rolling_ = false;
	//�_���[�W���󂯂��u��
	bool damaged_moment_ = false;

	//--------���C�������--------
	//�����̗�
	tnl::Vector3 move_vel_;
	//1�O�̓����̗�
	tnl::Vector3 past_move_vel_;
	//�d�S
	tnl::Vector3 center_of_gravity_;
	//----------------------------
public:
	//�e�̃��X�g
	std::list<std::shared_ptr<Bullet>> bullet_;

	//�Z�b�^�[
	void setTankPos(tnl::Vector3 tank_pos);
	void setTankDamaged(bool damaged);
	void setTankHp(int hp);
	void setTankDamagedMoment(bool damaged_moment);
	void setPlayerPointer(std::shared_ptr<PlayerTank> player);
	void setFloorPointer(std::shared_ptr<Floor> floor);
	void setWallPointer(std::shared_ptr<Wall> wall);
	void setSpawnerPointer(std::shared_ptr<Spawner> spawner);

	//�Q�b�^�[
	tnl::Vector3 getTankPos() const;
	tnl::Vector3 getTankSize() const;
	int getTankHp() const;
	bool getTankDead() const;
	bool getTankRolling() const;
	bool getTankDamaged() const;
};
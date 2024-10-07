#define _USE_MATH_DEFINES
#include "../../../../../../dxlib_ext/dxlib_ext.h"
#include "EnemyTank.h"
#include "../../player/PlayerTank.h"
#include "../../../../../gm_mgr/ResourceManager.h"
#include "../../../gm_obj/Bullet.h"
#include "../../../gm_obj/Wall.h"
#include "../../TankBase.h"

EnemyTank::EnemyTank(const tnl::Vector3& spawn_pos, const float& dir_angle, bool hard_mode) {
	tank_size_ = TANK_FORM * TANK_SIZE;
	tank_pos_ = spawn_pos;
	dir_angle_ = dir_angle;
	hard_mode_ = hard_mode;

	mesh_ = dxe::Mesh::CreateBoxMV(tank_size_,
		dxe::Texture::CreateFromFile("graphics/tank/enemy_tank_side.jpg"),
		dxe::Texture::CreateFromFile("graphics/tank/enemy_tank_side.jpg"),
		dxe::Texture::CreateFromFile("graphics/tank/enemy_tank_up.jpg"),
		dxe::Texture::CreateFromFile("graphics/tank/enemy_tank_down.jpg"),
		dxe::Texture::CreateFromFile("graphics/tank/enemy_tank_side_front.jpg"),
		dxe::Texture::CreateFromFile("graphics/tank/enemy_tank_side_front.jpg")
	);
	mesh_->scl_ = TANK_FORM;
	mesh_->pos_ = spawn_pos;
	mesh_->setCullingMode(DX_CULLING_RIGHT);

	//�ŏ��Ɉ�񃉃��_���œ�����
	RandomDirChange();

	//hp�̐ݒ�
	tank_hp_ = TANK_HP;
	//�e�̌��Ԋu�̒��I
	RandomShotCount();

	//�摜��ǂݍ���
	LoadCommonGraph();
	//���y�f�[�^��ǂݍ���
	LoadCommonSound();
	shot_snd_ = ResourceManager::GetInstance()->loadSound("sound/se/enemy_shot.mp3");
	dead_snd_ = ResourceManager::GetInstance()->loadSound("sound/se/enemy_dead.mp3");
	reflection_snd_ = ResourceManager::GetInstance()->loadSound("sound/se/reflection_shot.mp3");
}

EnemyTank::~EnemyTank() {

}

void EnemyTank::update(float delta_time) {
	//�V�[�P���X�̃A�b�v�f�[�g
	sequence_state_.update(delta_time);

	//HP�o�[�̌����ɗp���鎞�Ԍo��
	if (is_hp_counting_) {
		hp_elapsed_ += delta_time;
	}
	//-------------------------------�e�̃��X�g�̏���------------------------
	auto blt_it = bullet_.begin();
	while (blt_it != bullet_.end()) {
		//�e�������Ă���\�������邩��`�F�b�N
		if ((*blt_it) == nullptr) {
			continue;
		}
		//�e�̃��X�g�̍X�V����
		(*blt_it)->update(delta_time);

		//�e�����@�Ɠ���������
		if (TryBulletCollision(*blt_it)) {
			//��]���Ă�����
			if (player_->getTankRolling()) {
				//������ς���
				(*blt_it)->BulletDir(true);
				//���ʉ�
				PlaySoundMem(reflection_snd_, DX_PLAYTYPE_BACK);
			}
			else {
				//���@���_���[�W
				player_->setTankDamaged(true);
			}
		}
		if (TryBulletToWallCollision(*blt_it, wall_) ||	//�e���ǂɓ���������
			player_->getTankDamaged()) {				//���@���_���[�W���󂯂���
			blt_it = bullet_.erase(blt_it);
			continue;
		}
		blt_it++;
	}
}

bool EnemyTank::seqNotShooting(float delta_time) {
	//���Ԍo��
	random_elapsed_ += delta_time;
	//�����_���Ɍ�����ς��鎞�Ԃ̏���
	if (random_elapsed_ >= DIR_CHANGE_COUNT) {
		RandomDirChange();
		random_elapsed_ = 0;
	}
	//�p�x��dir_value�����ς���
	dir_angle_ += dir_value_;

	//-------------------------------���b�V���̍X�V-----------------------------------------
	//����
	mesh_->rot_ = tnl::Quaternion::RotationAxis(
		TANK_AXIS, tnl::ToRadian(static_cast<float> (dir_angle_)));
	//�i��
	//�i�ގw�߂��o�Ă��鎞
	if (go_judge_) {
		move_vel_ += tnl::Vector3::TransformCoord(TANK_DIR, mesh_->rot_);
	}
	//���ʂƖ��C�̌v�Z
	tnl::EasyAdjustObjectVelocity(CENTROID_RADIUS, MASS, FRICTION,
		past_move_vel_, move_vel_, center_of_gravity_);
	//��Ԃ̍��W�𓮂���
	tank_pos_ += move_vel_ * MOVE_SPEED;
	//���b�V���̐�Ԃ̈ʒu���X�V
	mesh_->pos_ = tank_pos_;
	
	//�e�𔭎˂���^�C�~���O�ɂȂ�����
	if (shot_elapsed_ >= shot_count_) {
		//�e�𔭎˂���܂ł̕b�������Z�b�g
		shot_elapsed_ = 0;
		//�󋵂��u�e�̔��˒��v�ɕς���
		sequence_shooting_.change(&EnemyTank::seqShooting);
	}
	return true;
}

bool EnemyTank::seqShooting(float delta_time) {
	if (sequence_shooting_.isStart()) {
		//���@�Ƃ̊p�x���v�Z
		AngleCalcToTank();
		//�e�̌��������@�Ɍ�����
		tnl::Vector3 move_dir = tnl::Vector3::TransformCoord(TANK_DIR,
			tnl::Quaternion::RotationAxis(TANK_AXIS, tnl::ToRadian(static_cast<float>(angle_to_tank_))));
		//���������@�Ɍ�����
		mesh_->rot_ = tnl::Quaternion::RotationAxis(TANK_AXIS, tnl::ToRadian(static_cast<float>(angle_to_tank_)));
		//�e����
		bullet_.emplace_back(std::make_shared<Bullet>(tank_pos_, move_dir, angle_to_tank_, true));
		//���ʉ�
		PlaySoundMem(shot_snd_, DX_PLAYTYPE_BACK);
		//�e�̌��Ԋu�̒��I
		RandomShotCount();
	}
	//�e�𔭎˂��Ă���K��̎��Ԃ��߂�����
	if (shot_elapsed_ >= SHOT_INTERVAL) {
		
		//�e�𔭎˂���܂ł̕b�������Z�b�g
		shot_elapsed_ = 0;
		//��Ԃ�߂�
		sequence_shooting_.change(&EnemyTank::seqNotShooting);
	}
	return true;
}

bool EnemyTank::seqNormal(float delta_time) {
	//�����Ă��鎞�̍X�V����
	UpdateAliveTank(delta_time);
	//�_���[�W���󂯂���
	if (damaged_moment_) {
		//���ʉ�
		PlaySoundMem(damage_snd_, DX_PLAYTYPE_BACK);
		//HP��1�ȏ�Ȃ�_���[�W���ɑJ��
		if (tank_hp_ > TANK_DAED_HP) {
			sequence_state_.change(&EnemyTank::seqDamaged);
		}
	}
	//hp���Ȃ��Ȃ����玀�S���֑J��
	if (tank_hp_ <= TANK_DAED_HP) {
		sequence_state_.change(&EnemyTank::seqDead);
	}
	return true;
}

bool EnemyTank::seqDamaged(float delta_time) {
	//�_���[�W���󂯂���
	if (damaged_moment_) {
		//��Ԃ�HP�����炷
		tank_hp_--;
		//hp�̃J�E���g���n�߂�
		is_hp_counting_ = true;
		//hp�o�[�������_�����݂�hp�o�[�̉E�[�ɕύX
		hp_bar_start_right = hp_bar_length_;
		//hp�o�[�̌o�ߎ��Ԃ̒l�����Z�b�g
		hp_elapsed_ = 0;
		//�_���[�W���󂯂��u�Ԃ̔�������Z�b�g
		damaged_moment_ = false;

		//�܂��A�_���[�W���Ƀ_���[�W���󂯂���
		if (!sequence_state_.isStart()) {
			//���ʉ�
			PlaySoundMem(damage_snd_, DX_PLAYTYPE_BACK);
		}
	}
	//�����Ă��鎞�̍X�V����
	UpdateAliveTank(delta_time); 
	//�K�莞�ԂŒʏ펞�ɑJ��
	if (hp_elapsed_ >= HP_ELAPSED_LIMIT) {
		is_hp_counting_ = false;
		sequence_state_.change(&EnemyTank::seqNormal);
	}
	//hp���Ȃ��Ȃ����玀�S���֑J��
	if (tank_hp_ <= TANK_DAED_HP) {
		sequence_state_.change(&EnemyTank::seqDead);
	}
	return true;
}

bool EnemyTank::seqDead(float delta_time) {
	if(sequence_state_.isStart()) {
		//hp�o�[�������_�����݂�hp�o�[�̉E�[�ɕύX
		hp_bar_start_right = hp_bar_length_;
		hp_elapsed_ = 0;
		is_hp_counting_ = true;
	}
	//hp������؂����玀�S
	if (hp_bar_right_ <= screen_hp_bar_pos_.x) {
		dead_ = true;
		//���ʉ�
		PlaySoundMem(dead_snd_, DX_PLAYTYPE_BACK);
	}
	return true;
}

void EnemyTank::UpdateAliveTank(float delta_time) {
	//���Ԍo��
	shot_elapsed_ += delta_time;
	//�e�̔��ˏ󋵂ɉ����čX�V����
	sequence_shooting_.update(delta_time);
}

void EnemyTank::draw(Shared<dxe::Camera> camera) {
	//���݂̃J�����ʒu���猩��HP�o�[�̈ʒu���o��
	screen_hp_bar_pos_ = tnl::Vector3::ConvertToScreen(
		{ tank_pos_.x - HP_BAR_HALF, HP_BAR_HEIGHT, tank_pos_.z },
		DXE_WINDOW_WIDTH,
		DXE_WINDOW_HEIGHT,
		camera->view_,
		camera->proj_);

	//���b�V���̕`��
	mesh_->render(camera);
	//�e�̕`��
	for (auto blt : bullet_) {
		blt->draw(camera);
	}
	//HP�o�[�̕`��
	
	//HP�o�[�̌v�Z
	HpCalc();
	int screen_hp_bar_pos_x = static_cast<int>(screen_hp_bar_pos_.x);
	int screen_hp_bar_pos_y = static_cast<int>(screen_hp_bar_pos_.y);
	//�w�i�̍����o�[
	DrawExtendGraph(screen_hp_bar_pos_x, screen_hp_bar_pos_y,
		screen_hp_bar_pos_x + BAR_WIDTH.x, screen_hp_bar_pos_y + BAR_WIDTH.y, hp_back_gfx_, false);
	//�΃o�[
	DrawExtendGraph(screen_hp_bar_pos_x, screen_hp_bar_pos_y,
		static_cast<int>(hp_bar_right_), screen_hp_bar_pos_y + BAR_WIDTH.y, hp_gfx_, false);
}

void EnemyTank::RandomShotCount() {
	//������[�h�̎��̒e�̌��Ԋu�̐ݒ�
	if (hard_mode_) {
		shot_count_ = SHOT_COUNT_AVG_HARD;
	}
	//�ȒP���[�h�̎��̒e�̌��Ԋu�̐ݒ�
	else {
		shot_count_ = SHOT_COUNT_AVG_EASY;
	}
	//�e�����Ԋu�̒��I�i-0.9����+1.0�̃����_�������j
	shot_count_ += (((rand() % RANDOM_SHOT_RANGE) - RANDOM_SHOT_OFFSET) * RANDOM_SHOT_SCALE);
}

void EnemyTank::RandomDirChange() {
	//�����������_���ŕς���
	dir_value_ = static_cast<float>((rand() % RANDOM_DIR_RANGE) - RANDOM_DIR_OFFSET) * RANDOM_DIR_POWER;
	//�i�ނ��~�܂邩�������_���Ō��߂�
	go_judge_ = rand() % RANDOM_GO_RANGE;
}

void EnemyTank::AngleCalcToTank() {
	//���@�Ƃ̋������v�Z
	distance_to_tank_.x = static_cast<int>(tank_pos_.x - player_->getTankPos().x);
	distance_to_tank_.y = static_cast<int>(tank_pos_.z - player_->getTankPos().z);
	//���@�Ƃ̊p�x���v�Z
	angle_to_tank_ = static_cast<int>(atan2(distance_to_tank_.x, distance_to_tank_.y) * RAD_TO_DEG) + ANGLE_OFFSET;
}

void EnemyTank::HpCalc() {
	//�������Ă���HP�o�[���ڎw���Ă��钷��
	float next_hp_bar_length = static_cast<float>(BAR_WIDTH.x) -
		((tank_hp_ - TANK_HP) * -(static_cast<float>(BAR_WIDTH.x) / TANK_HP));
	if (is_hp_counting_) {
		//hp�̌�������������Ă��猸��
		hp_bar_length_ = tnl::SmoothLerp(hp_bar_start_right, next_hp_bar_length, HP_ELAPSED_LIMIT, hp_elapsed_, SMOOTHING_ITERATIONS);
	}
	//HP�o�[��X���W�����ۂ̍��W�̈ʒu��
	hp_bar_right_ = screen_hp_bar_pos_.x + hp_bar_length_;
}

bool EnemyTank::TryBulletCollision(std::shared_ptr<Bullet> blt) {

	if (tnl::IsIntersectAABB(blt->getBulletPos(), blt->getBulletSize(), player_->getTankPos(), player_->getTankSize())
		) {
		return true;
	}
	else return false;
}
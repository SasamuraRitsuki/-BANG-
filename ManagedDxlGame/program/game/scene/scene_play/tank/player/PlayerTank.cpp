#include "../../../../../dxlib_ext/dxlib_ext.h"
#include "PlayerTank.h"
#include "../../../../gm_mgr/ResourceManager.h"
#include "../../gm_obj/Wall.h"
#include "../../gm_obj/Floor.h"
#include "../../gm_obj/Bullet.h"
#include "../TankBase.h"
#include "../enemy_spawner/Spawner.h"
#include "../enemy_spawner/enemy/EnemyTank.h"

PlayerTank::PlayerTank() {
	tank_size_ = TANK_FORM * TANK_SIZE;

	tank_pos_ = FIRST_POS;
	tank_hp_ = TANK_HP;
	mesh_ = dxe::Mesh::CreateBoxMV(tank_size_,
		dxe::Texture::CreateFromFile("graphics/tank/tank_side.jpg"),
		dxe::Texture::CreateFromFile("graphics/tank/tank_side.jpg"),
		dxe::Texture::CreateFromFile("graphics/tank/tank_up.jpg"),
		dxe::Texture::CreateFromFile("graphics/tank/tank_down.jpg"),
		dxe::Texture::CreateFromFile("graphics/tank/tank_side_front.jpg"),
		dxe::Texture::CreateFromFile("graphics/tank/tank_side_front.jpg")
	);
	mesh_->scl_ = TANK_FORM;
	mesh_->pos_ = FIRST_POS;
	mesh_->setCullingMode(DX_CULLING_RIGHT);

	//��]���\��Ԃɂ��Ă���
	rotatable_ = true;
	//�X�s���Q�[�W�̃`���[�W
	rolling_elapsed_ = ROTATABLE_TIME;
	//�摜��ǂݍ���
	LoadCommonGraph();
	spin_gfx_ = ResourceManager::GetInstance()->loadGraph("graphics/yellow1.jpg");
	back_gfx_ = ResourceManager::GetInstance()->loadGraph("graphics/white.bmp");
	bullet_gfx_ = ResourceManager::GetInstance()->loadGraph("graphics/bullet.png");
	//���y��ǂݍ���
	LoadCommonSound();
;	rolling_snd_ = ResourceManager::GetInstance()->loadSound("sound/se/rolling.mp3");
	jump_snd_ = ResourceManager::GetInstance()->loadSound("sound/se/jump.mp3");
	shot_snd_ = ResourceManager::GetInstance()->loadSound("sound/se/player_shot.mp3");
}

PlayerTank::~PlayerTank() {

}

void PlayerTank::update(float delta_time) {
	//�V�[�P���X�̃A�b�v�f�[�g
	sequence_.update(delta_time);

	//--------------------------------------hp�o�[�̏���------------------------------------------------
	if (hp_calc_) {
		hp_elapsed_ += delta_time;
	}
	//�������Ă���HP�o�[���ڎw���Ă��钷��
	float next_hp_bar_right = HP_GAUGE_POS.x + static_cast<float>((DXE_WINDOW_WIDTH * tank_hp_) / (TANK_HP * TANK_HP));

	//hp�̌�������������Ă��猸��
	hp_bar_right_ = tnl::SmoothLerp(hp_bar_right_, next_hp_bar_right, HP_ELAPSED_LIMIT, hp_elapsed_, SMOOTHING_ITERATIONS);

	if (hp_elapsed_ >= HP_ELAPSED_LIMIT) {
		hp_calc_ = false;
		hp_elapsed_ = 0;
	}
	
	//�d�͂ɂ�闎������
	jump_value_ -= GRAVITY_VALUE;
	//�W�����v�̒l�����W�ɔ��f
	tank_pos_.y += jump_value_;
	//���W�����b�V���ɔ��f
	mesh_->pos_ = tank_pos_;
}

bool PlayerTank::seqDead(const float delta_time) {
	if (!hp_calc_) {
		dead_ = true;
	}
	return true;
}

bool PlayerTank::seqNormal(const float delta_time) {
	//������Ă�������Ɉړ�
	DirChange();
	//--------------------------------------�e�̔���--------------------------------------------------------
	//�e���ő吔�܂ŏo�Ă��Ȃ���
	if (bullet_.size() <= BULLET_MAX - 1) {
		//�}�E�X�̍��W�̃`�F�b�N
		AngleCalcToMouse();
		//���N���b�N������
		if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_LEFT)) {
			//�e�̌��������@����}�E�X�ʒu�Ɍ�����
			tnl::Vector3 move_dir = tnl::Vector3::TransformCoord(TANK_DIR,
				tnl::Quaternion::RotationAxis(TANK_AXIS, tnl::ToRadian(static_cast<float>(angle_to_mouse_))));
			//�e�̐���
			bullet_.emplace_back(std::make_shared<Bullet>(tank_pos_, move_dir, angle_to_mouse_, false));
			//���ʉ�
			PlaySoundMem(shot_snd_, DX_PLAYTYPE_BACK);
		}
	}
	//----------------------------------��]����--------------------------------------------------
	if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_RIGHT) &&	//�E�N���b�N���������
		rotatable_) {											//��]�\�Ȏ���
		rolling_ = true;
		rotatable_ = false;
		rolling_elapsed_ = 0;
		//���ʉ�
		PlaySoundMem(rolling_snd_, DX_PLAYTYPE_BACK);
	}
	//��]���̎��̂�
	if (!rotatable_) {
		//��Ԃ̉�]����
		TankRolling(delta_time);
	}
	//-------------------------------------�W�����v�̏���-------------------------------------------------------
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE) &&//�X�y�[�X�L�[�������ꂽ��
		!falling) {										//����������Ȃ���
		//������ɒl�𑝉�
		jump_value_ = JUMP_VALUE;
		//�������ɂ���
		falling = true;
		//���ʉ�
		PlaySoundMem(jump_snd_, DX_PLAYTYPE_BACK);
	}

	//--------------------------------------�������ɏ�������̏���-------------------------------------------------------
	//���C�̌v�Z
	tnl::EasyAdjustObjectVelocity(CENTROID_RADIUS, MASS, FRICTION,
		past_move_vel_, move_vel_, center_of_gravity_);


	//�������̏���
	MoveFloorCalc(move_floor1_state_, tank_pos_.x, floor_->getDeltaMoveFloor1Distance(), move_vel_.x);
	MoveFloorCalc(move_floor2_state_, tank_pos_.x, floor_->getDeltaMoveFloor2Distance(), move_vel_.x);
	//��Ԃ𓮂���
	tank_pos_ += move_vel_ * MOVE_SPEED;

	//--------------------------------------------------�e�̏����Ȃ�-------------------------------------------------------------------

	auto blt_it = bullet_.begin();
	//�e���X�g���`�F�b�N
	while (blt_it != bullet_.end()) {
		//�e�̍X�V����
		(*blt_it)->update(delta_time);

		auto e_it = spawner_->enemy_.begin();
		//�e�������Ă��邩
		bool is_alive_blt = false;
		// �G���X�g���`�F�b�N
		while (e_it != spawner_->enemy_.end()) {
			// �e�ƓG�̓����蔻��̏���
			if (TryEnemyCollision((*blt_it), (*e_it)->getTankPos(), (*e_it)->getTankSize())) {
				//�_���[�W��^����
				(*e_it)->setTankDamagedMoment(true);
				//�e�������t���O�𗧂Ă�
				is_alive_blt = true;
			}
			e_it++;
		}
		if (is_alive_blt ||								//�e�������ĂȂ�������
			TryBulletToWallCollision(*blt_it, wall_) ||	//�ǂɓ���������
			damaged_) {									//�_���[�W��H�������
			blt_it = bullet_.erase(blt_it);
			is_alive_bullet_ = false;
			continue;
		}
		blt_it++;
	}

	//----------------------------------�����ʒu�ɖ߂�-------------------------------------------
	auto e_it = spawner_->enemy_.begin();
	while (e_it != spawner_->enemy_.end()) {
		//�G�Ǝ��@�Ƃ̓����蔻��
		//�Ԃ�������_���[�W
		if (tnl::IsIntersectAABB((*e_it)->getTankPos(), (*e_it)->getTankSize(), tank_pos_, tank_size_)) {
			damaged_ = true;
			break;
		}
		e_it++;
	}

	if (tank_pos_.y <= DEAD_Y ||//����������
		damaged_) {				//�_���[�W�󂯂���
		if (tank_hp_ > TANK_DAED_HP + 1) {		//HP���c��1����Ȃ����͏����l�ɖ߂�
			tank_pos_ = FIRST_POS;
			mesh_->rot_ = FIRST_ROT;
			jump_value_ = 0;
		}
		move_floor1_state_.is_riding_ = false;
		move_floor2_state_.is_riding_ = false;
		damaged_ = false;
		hp_calc_ = true;
		tank_hp_--;
		//���ʉ�
		PlaySoundMem(damage_snd_, DX_PLAYTYPE_BACK);
	}

	//HP���Ȃ��Ȃ����玀�S���ɑJ��
	if (tank_hp_ <= 0) {
		sequence_.change(&PlayerTank::seqDead);
	}
	return true;
}

void PlayerTank::draw(Shared<dxe::Camera> camera) {

	//�e�̕`��
	mesh_->render(camera);
	for (auto blt : bullet_) {
		blt->draw(camera);
	}

	//���̔��������w�i
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, TRANSPARENCY_WHITE_BACK);
	//����̒e�̌��
	DrawExtendGraph(UP_BACK_POS_FIRST.x, UP_BACK_POS_FIRST.y,
		UP_BACK_POS_LAST.x, UP_BACK_POS_LAST.y, back_gfx_, false);
	//���̃o�[�̌��
	DrawExtendGraph(DOWN_BACK_POS_FIRST.x, DOWN_BACK_POS_FIRST.y,
		DOWN_BACK_POS_LAST.x, DOWN_BACK_POS_LAST.y, back_gfx_, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, TRANSPARENCY_MAX);

	//�w�i�̍����o�[
	DrawExtendGraph(HP_GAUGE_POS.x, HP_GAUGE_POS.y,
		HP_GAUGE_POS.x + BAR_WIDTH.x, HP_GAUGE_POS.y + BAR_WIDTH.y, hp_back_gfx_, false);
	//�΃o�[
	DrawExtendGraph(HP_GAUGE_POS.x, HP_GAUGE_POS.y,
		static_cast<int>(hp_bar_right_), HP_GAUGE_POS.y + BAR_WIDTH.y, hp_gfx_, false);

	//�w�i�̍����o�[
	DrawExtendGraph(SPIN_GAUGE_POS.x, SPIN_GAUGE_POS.y,
		SPIN_GAUGE_POS.x + BAR_WIDTH.x, SPIN_GAUGE_POS.y + BAR_WIDTH.y, hp_back_gfx_, false);
	//���o�[
	DrawExtendGraph(SPIN_GAUGE_POS.x, SPIN_GAUGE_POS.y,
		static_cast<int>(spin_bar_right_), SPIN_GAUGE_POS.y + BAR_WIDTH.y, spin_gfx_, false);

	//�����\��
	DrawStringEx(HP_TEXT_POS.x, HP_TEXT_POS.y, TEXT_COLOR, "HP");
	DrawStringEx(SPIN_TEXT_POS.x, SPIN_TEXT_POS.y, TEXT_COLOR, "SPIN");
	DrawStringEx(SPIN_CLICK_TEXT_POS.x, SPIN_CLICK_TEXT_POS.y, TEXT_COLOR, "�E�N���b�N");

	int blt_remains = BULLET_MAX - static_cast<int>(bullet_.size());
	//����̒e�̕`��
	for (int blt = 0; blt < blt_remains; blt++) {
		DrawRotaGraph(BULLET_GFX_POS.x + blt * BULLET_WIDTH, BULLET_GFX_POS.y,
			BULLET_GFX_SIZE_SCALE, BULLET_GFX_ANGLE, bullet_gfx_, true);
	}

}

void PlayerTank::LandingOnTheFloor() {
	jump_value_ = 0;
	falling = false;
}

void PlayerTank::DirChange() {
	//���͂���Ă�������ւ̉��̃x�N�g���̒l
	tnl::Vector2i move_dir_vector = INITIAL_MOVE_VECTOR;
	
	//����������������Ă��鎞
	bool horizontal_input = false;
	//����������������Ă��鎞
	bool vertical_input = false;

// ���͂ɉ����ĉ��̈ړ��x�N�g�� (move_dir_vector) �̒l���X�V����
// 2�̕��s�ȕ��� (�㉺�A���E) �������ɉ�����Ă���ꍇ�́A�����Ȃ��悤�ɂ���B
// �������A3�ȏ�̕�����������Ă���ꍇ�́A�ړ���������B
	if (tnl::Input::IsKeyDown(eKeys::KB_W)) {
		move_dir_vector.x++;
		vertical_input = !vertical_input;
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_S)) {
		move_dir_vector.x--;
		vertical_input = !vertical_input;
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_D)) {
		move_dir_vector.y++;
		horizontal_input = !horizontal_input;
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_A)) {
		move_dir_vector.y--;
		horizontal_input = !horizontal_input;
	}
	//�x�N�g���̒l����p�x���o���B
	int dir_angle = static_cast<int>(atan2(move_dir_vector.y, move_dir_vector.x) * RAD_TO_DEG);

	//�����Ă�������ւ̑O�i�̒l���o��
	tnl::Quaternion now_rot_ = tnl::Quaternion::RotationAxis(
		TANK_AXIS, tnl::ToRadian(static_cast<float>(dir_angle)));

	//���E���㉺�̃{�^����������Ă鎞�͑O�i����
	if (horizontal_input || vertical_input) {
		move_vel_ += tnl::Vector3::TransformCoord(TANK_DIR, now_rot_);
		//��]���͌����ڂ͂����ł͓������Ȃ�
		if (!rolling_) {
			mesh_->rot_ = now_rot_;
		}
	}
}

void PlayerTank::AngleCalcToMouse() {
	//���S��0�Ƃ��ă}�E�X�̍��W�����
	mouse_pos_.x = tnl::Input::GetMousePosition().x - WINDOW_HALF_WIDTH;
	mouse_pos_.y = tnl::Input::GetMousePosition().y - WINDOW_HALF_HEIGHT;
	//���S����}�E�X�܂ł̊p�x
	angle_to_mouse_ = static_cast<int>(atan2(mouse_pos_.y, mouse_pos_.x) * RAD_TO_DEG) + ANGLE_OFFSET;
}

void PlayerTank::TankRolling(float delta_time) {
	rolling_elapsed_ += delta_time;

	// ���K��
	float normalizedTime = static_cast<float>(rolling_elapsed_) / ROLLING_TIME;

	// �C�[�W���O�֐���K�p
	float easedTime = EaseInOut(normalizedTime);


	//���邮���]
	if (rolling_) {
		// y �̕ω�
		rolling_speed_ = easedTime * 30.0f;
		mesh_->rot_ *= tnl::Quaternion::RotationAxis(TANK_AXIS, tnl::ToRadian(rolling_speed_));
	}

	if (rolling_elapsed_ >= ROLLING_TIME && rolling_) {
		rolling_ = false;//��]����off��
	}
	if (rolling_elapsed_ >= ROTATABLE_TIME) {
		rolling_elapsed_ = ROTATABLE_TIME;
		rotatable_ = true;//��]�\��on��
	}
	//��]�̃Q�[�W�̉E�[�̌v�Z
	spin_bar_right_ = SPIN_BAR_SPEED * rolling_elapsed_ * (DXE_WINDOW_WIDTH / ROLLING_TIME) + SPIN_GAUGE_POS.x;
}

float PlayerTank::EaseInOut(float t) {
	// t �͎��ԁi0.0 ���� 1.0 �͈̔͂ŕω�����l�j

	// �����������^���𗘗p�����l�̕��
	float easedValue = tnl::DecelLerp(0, 1, 1, t);

	return easedValue;
}

void PlayerTank::MoveFloorCalc(MoveFloorState& floor_state, float& tank_pos_x, float delta_move_floor_distance,float move_vel) {

	if (floor_state.is_riding_) {
		// �������ɏ��n�߂��ŏ��̃t���[���̏���
		if (floor_state.just_started_riding_) {
			//������n�_�̍��W���L��
			floor_state.prev_tank_pos_x_ = tank_pos_x;
			floor_state.prev_delta_move_floor_distance_ = delta_move_floor_distance;
			// �ŏ��̃t���[���t���O���I�t�ɂ���
			floor_state.just_started_riding_ = false;  
		}
		//���œ�����悤�ɂ���+-
		floor_state.prev_tank_pos_x_ += move_vel_.x * MOVE_SPEED;
		// ��Ԃ̈ʒu���X�V����
		tank_pos_x = floor_state.prev_tank_pos_x_ + delta_move_floor_distance - floor_state.prev_delta_move_floor_distance_;
	}
	else {
		// ������~�肽�Ƃ��A���ɏ��Ƃ��̂��߂ɍŏ��̃t���[���t���O���I���ɂ���
		floor_state.just_started_riding_ = true;
	}
}

bool PlayerTank::TryEnemyCollision(std::shared_ptr<Bullet> blt, tnl::Vector3 enemy_pos, tnl::Vector3 enemy_size) {

	if (tnl::IsIntersectAABB(enemy_pos, enemy_size, blt->getBulletPos(), blt->getBulletSize())
		) {
		return true;
	}
	else return false;
}

void PlayerTank::setIsRidingMoveFloor1(bool is_riding) {
	move_floor1_state_.is_riding_ = is_riding;
}

void PlayerTank::setIsRidingMoveFloor2(bool is_riding) {
	move_floor2_state_.is_riding_ = is_riding;
}
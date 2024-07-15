#pragma once
#include "../TankBase.h"

class Bullet;

struct MoveFloorState {
	// �������ɏ���Ă��邩�ǂ����������t���O
	bool is_riding_;               
	// �������ɏ��n�߂��ŏ��̃t���[�����ǂ����������t���O
	bool just_started_riding_;
	// �������ɏ��O�̐�Ԃ̈ʒu
	float prev_tank_pos_x_;
	// �������ɏ��O�̏��̈ʒu
	float prev_delta_move_floor_distance_;

	// �R���X�g���N�^�ŏ����l��ݒ�
	MoveFloorState()
		: is_riding_(false), just_started_riding_(true), prev_tank_pos_x_(0.0f), prev_delta_move_floor_distance_(0.0f) {}
};

class PlayerTank : public TankBase{
public:
	PlayerTank();
	~PlayerTank();

	void update(float delta_time) override;
	void draw(Shared<dxe::Camera> camera) override;

	//�G�ƒe�̓����蔻��
	bool TryEnemyCollision(std::shared_ptr<Bullet> blt, tnl::Vector3 enemy_pos, tnl::Vector3 enemy_size);
	//���n���̏���
	void LandingOnTheFloor(); 

	//�Z�b�^�[
	void setIsRidingMoveFloor1(bool is_riding);
	void setIsRidingMoveFloor2(bool is_riding);

private:
	//���݂̏��
	tnl::Sequence<PlayerTank> sequence_ = tnl::Sequence<PlayerTank>(this, &PlayerTank::seqNormal);
	//�ʏ펞
	bool seqNormal(float delta_time);
	//���S��
	bool seqDead(float delta_time);

	//������ς���
	void DirChange();
	//�}�E�X�ւ̊p�x�v�Z
	void AngleCalcToMouse();
	//��]����
	void TankRolling(float delta_time);
	//�������v�Z
	float EaseInOut(float t);
	//�������̏����v�Z
	void MoveFloorCalc(MoveFloorState& floor_state, float& tank_pos, float delta_move_floor_pos,float move_vel);

	// ������1�ɏ���Ă�����
	MoveFloorState move_floor1_state_;
	// ������2�ɏ���Ă�����
	MoveFloorState move_floor2_state_;


	//�X�^�[�g�ʒu
	const tnl::Vector3 FIRST_POS = { 0,50,0 };
	//tank�̌`
	const tnl::Vector3 TANK_FORM = { 1.0f, 0.75f, 1.0f };
	//�e�̉摜�̈ʒu�Ƒ傫��
	const tnl::Vector3 BULLET_POS = { 32, 50, 0.8f };
	//HP�̃Q�[�W�ʒu
	const tnl::Vector2i HP_GAUGE_POS = { DXE_WINDOW_WIDTH/3 ,DXE_WINDOW_HEIGHT * 4 / 5 };
	//�X�s���Q�[�W�ʒu
	const tnl::Vector2i SPIN_GAUGE_POS = { HP_GAUGE_POS.x ,576 + 60 };
	//�e�L�X�g�̈ʒu
	const tnl::Vector2i HP_TEXT_POS = { HP_GAUGE_POS.x - 90, HP_GAUGE_POS.y };
	const tnl::Vector2i SPIN_TEXT_POS = { SPIN_GAUGE_POS.x - 150, SPIN_GAUGE_POS.y - 5 };
	const tnl::Vector2i SPIN_CLICK_TEXT_POS = { SPIN_TEXT_POS.x + 600, SPIN_TEXT_POS.y };
	//���̔����w�i�̈ʒu�̍���̈ʒu
	const tnl::Vector2i DOWN_BACK_POS_FIRST = { 140,HP_GAUGE_POS.y - 20 };
	//���̔����w�i�̈ʒu�̉E���̈ʒu
	const tnl::Vector2i DOWN_BACK_POS_LAST = { DXE_WINDOW_WIDTH - 140,SPIN_GAUGE_POS.y + 60 };
	//��̔����w�i�̈ʒu�̍���̈ʒu
	const tnl::Vector2i UP_BACK_POS_FIRST = { 0,0 };
	//��̔����w�i�̈ʒu�̉E���̈ʒu
	const tnl::Vector2i UP_BACK_POS_LAST = { 64 * 5,104 };
	//�ړ���
	const float MOVE_SPEED = 5;
	//tank�̑傫��
	const float TANK_SIZE = 80;
	//�W�����v��
	const float JUMP_VALUE = 10;
	//�d�͂̏����l
	const float GRAVITY_VALUE = 0.49f;
	//��]����
	const float ROLLING_TIME = 0.8f;
	//��]�\����
	const float ROTATABLE_TIME = ROLLING_TIME * 2;
	//hp�����肫�鎞��
	const float HP_ELAPSED_LIMIT = 1;
	//����HP
	const int TANK_HP = 3;
	//�������S���C��
	const int DEAD_Y = -500;
	//�e�̔��ˉ\��
	const int BULLET_MAX = 5;
	//��]���x
	const int ROLLING_SPEED = 25;
	//�e�L�X�g�̃J���[�R�[�h
	const int TEXT_COLOR = 0;

	//�}�E�X�̍��W
	tnl::Vector3 mouse_pos_;
	//�d�͂ɂ�鐂�������̑傫���̒l
	float jump_value_ = 0;
	//��]�̌o�ߎ���
	float rolling_elapsed_ = 0;
	//HP�o�[�̉E�[
	float hp_bar_right_ = static_cast<float>(HP_GAUGE_POS.x + HP_GAUGE_POS.x);
	//�X�s���o�[�̉E�[
	float spin_bar_right_ = static_cast<float>(SPIN_GAUGE_POS.x + HP_GAUGE_POS.x);
	//��]�̑���
	float rolling_speed_ = 0;
	//hp�o�[�̌o�ߎ���
	float hp_elapsed_ = 0;
	//�}�E�X�ւ̊p�x
	int angle_to_mouse_;
	//�}�E�X�ŃN���b�N�������ǂ���
	bool clicked_ = false;
	//1�N���b�N��1����������
	bool shoted_ = false;
	//�e�������锻����󂯂���
	bool is_alive_bullet_ = false;
	//���������ǂ���
	bool falling = false;
	//��]�\���ǂ���
	bool rotatable_ = false;
	//hp�v�Z��
	bool hp_calc_ = false;
	//hp��0�ɂȂ������ɓ������~�߂�
	bool move_stop_ = false;

	//-----------------------�摜�≹�y�p�X-------------------------------
	//�X�s���Q�[�W�̉摜�p�X
	int spin_gfx_;
	//���̔����w�i�̉摜�p�X
	int back_gfx_;
	//�e�̉摜�p�X
	int bullet_gfx_;
	//��]�̉��y�p�X
	int rolling_snd_;
	//�W�����v�̉��y�p�X
	int jump_snd_;
};
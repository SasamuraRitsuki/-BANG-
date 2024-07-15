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
	//���݂̏��
	tnl::Sequence<EnemyTank> sequence_state_ = tnl::Sequence<EnemyTank>(this, &EnemyTank::seqNormal);
	//���݂̒e�������Ă��邩�ǂ����̏��
	tnl::Sequence<EnemyTank> sequence_shooting_ = tnl::Sequence<EnemyTank>(this, &EnemyTank::seqNotShooting);
	//�u���݂̏�ԁv��p�̃V�[�P���X
	//�ʏ펞
	bool seqNormal(float delta_time);
	//�_���[�W��
	bool seqDamaged(float delta_time);
	//���S��
	bool seqDead(float delta_time);
	//�u���݂̒e�������Ă��邩�ǂ����̏�ԁv��p�̃V�[�P���X
	//�e�������Ă��Ȃ�
	bool seqNotShooting(float delta_time);
	//�e�̔��˒�
	bool seqShooting(float delta_time);

	//�������̍X�V����
	void UpdateAliveTank(float delta_time);
	//�����_���Œe�����Ԋu�𒊑I����
	void RandomShotCount();
	//�����_���Ō�����ς���
	void RandomDirChange();
	//��Ԃւ̊p�x�v�Z
	void AngleCalcToTank();
	//Hp�̌v�Z
	void HpCalc();
	//�e�Ƃ̓����蔻��
	bool TryBulletCollision(std::shared_ptr<Bullet> blt);

	//tank�̌`
	const tnl::Vector3 TANK_FORM = { 1.0f, 0.75f, 1.0f };
	//tank�̑傫��
	const float TANK_SIZE = 80;
	//�����̔{��
	const float RANDOM_DIR_POWER = 0.5f;
	//�d�͂̏����l
	const float GRAVITY_VALUE = 0.49f;
	//hp�����肫�鎞��
	const float HP_ELAPSED_LIMIT = 0.25f;
	//�ړ���
	const float MOVE_SPEED = 3;
	//�e�������Ă��瓮���o���܂ł̕b��
	const float SHOT_INTERVAL = 1.0f;
	//HP�o�[�̒���
	const float HP_BAR_MAX = 60;
	//������[�h�̒e�̌��Ԋu�̕��ϒl
	const float SHOT_COUNT_AVG_HARD = 4.0f;
	//�ȒP���[�h�̒e�̌��Ԋu�̕��ϒl
	const float SHOT_COUNT_AVG_EASY = 6.0f;
	//����HP
	const int TANK_HP = 3;
	//�W�����v��
	const int JUMP_VALUE = 10;
	//�������S���C��
	const int DEAD_Y = -500;
	//�e�̔��ˉ\��
	const int BULLET_MAX = 10;
	//�����_���Ō�����ς���b��
	const int DIR_CHANGE_COUNT = 1;

	//��Ԃւ̋���
	tnl::Vector2i distance_to_tank_;
	//��ʏ��hp�o�[�̍��W
	tnl::Vector3 screen_hp_bar_pos_;
	//�e�𔭎˂���܂ł̕b��
	float shot_elapsed_ = 0;
	//�����_���œ�����ς���܂ł̕b��
	float random_elapsed_ = 0;
	//HP�o�[�̉E�[
	float hp_bar_right_ = screen_hp_bar_pos_.x + HP_BAR_MAX;
	//hp�o�[�̌o�ߎ���
	float hp_elapsed_ = 0;
	//hp�o�[�̒���
	float hp_bar_length_ = HP_BAR_MAX;
	//hp�o�[�̉E�[�̎n���ʒu
	float hp_bar_start_right = HP_BAR_MAX;
	//�e�����܂ł̕b��(�����_���ɕς��)
	float shot_count_ = 0;
	//�����p�x�̒l
	float dir_value_ = 0;
	//�����Ă���p�x
	float dir_angle_ = 0;
	//��Ԃւ̊p�x
	int angle_to_tank_ = 0;
	//�i�ނ��~�܂邩
	bool go_judge_ = false;
	//hp�̃J�E���g��
	bool is_hp_counting_ = false;
	//������[�h���ǂ���
	bool hard_mode_ = false;

	//-----------------------���y�p�X-------------------------------
	//���ꂽ���̉��y�p�X
	int dead_snd_;
	//���˂̉��y�p�X
	int reflection_snd_;
};
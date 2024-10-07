#pragma once

class Floor {
public:
	Floor(bool hard_mode);
	~Floor();

	void update(float delta_time);
	void draw(Shared<dxe::Camera> camera);

	//�Q�b�^�[
	tnl::Vector3 getFloor1Pos() const {
		return floor1_pos_;
	}
	tnl::Vector3 getFloor2Pos() const {
		return floor2_pos_;
	}
	tnl::Vector3 getMoveFloor1Pos() const {
		return move_floor1_pos_;
	}
	tnl::Vector3 getMoveFloor2Pos() const {
		return move_floor2_pos_;
	}
	tnl::Vector3 getFloor1Size() const {
		return floor1_size_v;
	}
	tnl::Vector3 getFloor2Size() const {
		return floor2_size_v;
	}
	tnl::Vector3 getMoveFloorSize() const {
		return move_floor_size_v;
	}

	float getDeltaMoveFloor1Distance() const {
		return delta_move_floor1_distance_;
	}
	float getDeltaMoveFloor2Distance() const {
		return delta_move_floor2_distance_;
	}


private:
	//���̑傫��
	const float FLOOR_SIZE = 1000.0f;
	//�������̑傫��
	const float MOVE_FLOOR_SIZE = 250.0f;

	//--------------------------------��1(��O)-----------------------------------------
	//���̌`
	const tnl::Vector3 FLOOR1_FORM = { 1.7f, 0.25f, 1.0f };
	//���̍��W
	tnl::Vector3 floor1_pos_ = { 650.0f, -155.0f, 300.0f };
	//����p�̏��̑傫��
	tnl::Vector3 floor1_size_v;
	//���b�V��
	Shared<dxe::Mesh> floor1_mesh_ = nullptr;
	//--------------------------------��2(��)-----------------------------------------
	//���̌`
	const tnl::Vector3 FLOOR2_FORM = { 1.7f, 0.25f, 0.575f };
	//���̍��W
	tnl::Vector3 floor2_pos_ = { 650.0f, -155.0f, 2012.4f };
	//����p�̏��̑傫��
	tnl::Vector3 floor2_size_v;
	//���b�V��
	Shared<dxe::Mesh> floor2_mesh_ = nullptr;

	//--------------------------------������-----------------------------------------
	//���̌`
	const tnl::Vector3 MOVE_FLOOR_FORM = { 1.0f, 0.25f, 1.0f };
	//�����ǂ̂��炢������
	const int MOVE_FLOOR_DISTANCE = 600;
	//�ȒP���[�h�̎��A�����ǂꂭ�炢�̑����œ�����
	const float MOVE_FLOOR_SPEED_EASY = 0.4f;
	//������[�h�̎��A�����ǂꂭ�炢�̑����œ�����
	const float MOVE_FLOOR_SPEED_HARD = 0.6f;
	//����p�̏��̑傫��
	tnl::Vector3 move_floor_size_v;
	//�����ǂꂭ�炢�̑����œ�����
	float move_floor_speed_;
	//�o�ߎ���
	float flashed_value_ = 0;
	//--------------------------------������1(��O)-----------------------------------------
	//���̏������W
	tnl::Vector3 MOVE_FLOOR1_POS = { 650.0f,-61.0f,1050.0f };
	//���̍��W
	tnl::Vector3 move_floor1_pos_ = MOVE_FLOOR1_POS;
	//�����ʒu���瓮����������1�̈ړ�����
	float delta_move_floor1_distance_ = 0.0f;
	//���b�V��
	Shared<dxe::Mesh> move_floor1_mesh_ = nullptr;
	//--------------------------------������2(��)-----------------------------------------
	//���̏������W
	tnl::Vector3 MOVE_FLOOR2_POS = { 650.0f,-61.0f,1400.0f };
	//���̍��W
	tnl::Vector3 move_floor2_pos_ = MOVE_FLOOR2_POS;
	//�����ʒu���瓮����������2�̈ړ�����
	float delta_move_floor2_distance_ = 0.0f;
	//���b�V��
	Shared<dxe::Mesh> move_floor2_mesh_ = nullptr;



};
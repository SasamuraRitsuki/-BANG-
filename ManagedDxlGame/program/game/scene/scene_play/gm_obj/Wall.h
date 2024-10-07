#pragma once

class Wall {
public:
	Wall();
	~Wall();

	void update(float delta_time);
	void draw(Shared<dxe::Camera> camera);

	//�Q�b�^�[
	tnl::Vector3 getWall1Pos() const {
		return wall1_pos_;
	}
	tnl::Vector3 getWall2Pos() const {
		return wall2_pos_;
	}
	tnl::Vector3 getWall3Pos() const {
		return wall3_pos_;
	}
	tnl::Vector3 getWall4Pos() const {
		return wall4_pos_;
	}
	tnl::Vector3 getWall5Pos() const {
		return wall5_pos_;
	}
	tnl::Vector3 getWallVirtualPos() const {
		return wall_virtual_pos_;
	}
	tnl::Vector3 getWall1Size() const {
		return wall1_size_v;
	}
	tnl::Vector3 getWall2Size() const {
		return wall2_size_v;
	}
	tnl::Vector3 getWall3Size() const {
		return wall3_size_v;
	}
	tnl::Vector3 getWall4Size() const {
		return wall4_size_v;
	}
	tnl::Vector3 getWall5Size() const {
		return wall5_size_v;
	}
	tnl::Vector3 getWallVirtualSize() const {
		return wall_virtual_size_v;
	}

private:
	//�[�̕ǂ̑傫��
	const float WALL_SIZE = 100.0f;

	//--------------------------------��1��-----------------------------------------
	//�ǂ̌`
	const tnl::Vector3 WALL1_FORM = { 1.0f, 5.0f, 27.0f };
	//�ǂ̍��W
	const tnl::Vector3 wall1_pos_ = { -250.0f,-45.0f,1050.0f };
	//����p�̏��̑傫��
	tnl::Vector3 wall1_size_v;
	//���b�V��
	Shared<dxe::Mesh> wall1_mesh_ = nullptr;
	//--------------------------------��2�E-----------------------------------------
	//�ǂ̌`
	const tnl::Vector3 WALL2_FORM = WALL1_FORM;
	//�ǂ̍��W
	const tnl::Vector3 wall2_pos_ = { 1550.0f,-45.0f,1050.0f };
	//����p�̏��̑傫��
	tnl::Vector3 wall2_size_v;
	//���b�V��
	Shared<dxe::Mesh> wall2_mesh_ = nullptr;

	//--------------------------------��3��O-----------------------------------------
	//�ǂ̌`
	const tnl::Vector3 WALL3_FORM = { 17.0f, 5.0f, 1.0f };
	//�ǂ̍��W
	const tnl::Vector3 wall3_pos_ = { 650.0f, -45.0f, -250.0f };
	//����p�̏��̑傫��
	tnl::Vector3 wall3_size_v;
	//���b�V��
	Shared<dxe::Mesh> wall3_mesh_ = nullptr;

	//--------------------------------��4��-----------------------------------------
	//�ǂ̌`
	const tnl::Vector3 WALL4_FORM = WALL3_FORM;
	//�ǂ̍��W
	const tnl::Vector3 wall4_pos_ = { 650.0f, -45.0f, 2350.0f };
	//����p�̏��̑傫��
	tnl::Vector3 wall4_size_v;
	//���b�V��
	Shared<dxe::Mesh> wall4_mesh_ = nullptr;

	//--------------------------------��5���������̉���-----------------------------------------
	//�ǂ̌`
	const tnl::Vector3 WALL5_FORM = { 10.0f, 1.4f, 1.0f };
	//�ǂ̍��W
	const tnl::Vector3 wall5_pos_ = { 302.0f, 50.0f, 300.0f };
	//����p�̏��̑傫��
	tnl::Vector3 wall5_size_v;
	//���b�V��
	Shared<dxe::Mesh> wall5_mesh_ = nullptr;

	//--------------------------------���z��-----------------------------------------
	//�ǂ̌`
	const tnl::Vector3 WALL_VIRTUAL_FORM = { 19.0f, 5.0f, 8.4f };
	//���z�ǂ̍��W
	const tnl::Vector3 wall_virtual_pos_ = { 650.0f, -45.0f, 1264.0f };
	//����p�̏��̑傫��
	tnl::Vector3 wall_virtual_size_v;

};
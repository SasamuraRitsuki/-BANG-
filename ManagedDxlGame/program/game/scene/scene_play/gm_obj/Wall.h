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
	const float WALL_SIZE = 100;

	//--------------------------------��1��-----------------------------------------
	//�ǂ̌`
	const tnl::Vector3 WALL1_FORM = { 1, 5, 27 };
	//�ǂ̍��W
	const tnl::Vector3 wall1_pos_ = { -250,-45,1050 };
	//����p�̏��̑傫��
	tnl::Vector3 wall1_size_v;
	//���b�V��
	Shared<dxe::Mesh> wall1_mesh_ = nullptr;
	//--------------------------------��2�E-----------------------------------------
	//�ǂ̌`
	const tnl::Vector3 WALL2_FORM = WALL1_FORM;
	//�ǂ̍��W
	const tnl::Vector3 wall2_pos_ = { 1550,-45,1050 };
	//����p�̏��̑傫��
	tnl::Vector3 wall2_size_v;
	//���b�V��
	Shared<dxe::Mesh> wall2_mesh_ = nullptr;

	//--------------------------------��3��O-----------------------------------------
	//�ǂ̌`
	const tnl::Vector3 WALL3_FORM = { 17, 5, 1 };
	//�ǂ̍��W
	const tnl::Vector3 wall3_pos_ = { 650,-45,-250 };
	//����p�̏��̑傫��
	tnl::Vector3 wall3_size_v;
	//���b�V��
	Shared<dxe::Mesh> wall3_mesh_ = nullptr;

	//--------------------------------��4��-----------------------------------------
	//�ǂ̌`
	const tnl::Vector3 WALL4_FORM = WALL3_FORM;
	//�ǂ̍��W
	const tnl::Vector3 wall4_pos_ = { 650,-45,2350 };
	//����p�̏��̑傫��
	tnl::Vector3 wall4_size_v;
	//���b�V��
	Shared<dxe::Mesh> wall4_mesh_ = nullptr;

	//--------------------------------��5���������̉���-----------------------------------------
	//�ǂ̌`
	const tnl::Vector3 WALL5_FORM = { 10, 1.4f, 1 };
	//�ǂ̍��W
	const tnl::Vector3 wall5_pos_ = { 302,50,300 };
	//����p�̏��̑傫��
	tnl::Vector3 wall5_size_v;
	//���b�V��
	Shared<dxe::Mesh> wall5_mesh_ = nullptr;

	//--------------------------------���z��-----------------------------------------
	//�ǂ̌`
	const tnl::Vector3 WALL_VIRTUAL_FORM = { 19, 5, 8.4f };
	//���z�ǂ̍��W
	const tnl::Vector3 wall_virtual_pos_ = { 650,-45,1264 };
	//����p�̏��̑傫��
	tnl::Vector3 wall_virtual_size_v;

};
#pragma once

class Wall {
public:
	Wall();
	~Wall();

	void update(float delta_time);
	void draw(Shared<dxe::Camera> camera);

	//ゲッター
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
	//端の壁の大きさ
	const float WALL_SIZE = 100.0f;

	//--------------------------------壁1左-----------------------------------------
	//壁の形
	const tnl::Vector3 WALL1_FORM = { 1.0f, 5.0f, 27.0f };
	//壁の座標
	const tnl::Vector3 wall1_pos_ = { -250.0f,-45.0f,1050.0f };
	//判定用の床の大きさ
	tnl::Vector3 wall1_size_v;
	//メッシュ
	Shared<dxe::Mesh> wall1_mesh_ = nullptr;
	//--------------------------------壁2右-----------------------------------------
	//壁の形
	const tnl::Vector3 WALL2_FORM = WALL1_FORM;
	//壁の座標
	const tnl::Vector3 wall2_pos_ = { 1550.0f,-45.0f,1050.0f };
	//判定用の床の大きさ
	tnl::Vector3 wall2_size_v;
	//メッシュ
	Shared<dxe::Mesh> wall2_mesh_ = nullptr;

	//--------------------------------壁3手前-----------------------------------------
	//壁の形
	const tnl::Vector3 WALL3_FORM = { 17.0f, 5.0f, 1.0f };
	//壁の座標
	const tnl::Vector3 wall3_pos_ = { 650.0f, -45.0f, -250.0f };
	//判定用の床の大きさ
	tnl::Vector3 wall3_size_v;
	//メッシュ
	Shared<dxe::Mesh> wall3_mesh_ = nullptr;

	//--------------------------------壁4奥-----------------------------------------
	//壁の形
	const tnl::Vector3 WALL4_FORM = WALL3_FORM;
	//壁の座標
	const tnl::Vector3 wall4_pos_ = { 650.0f, -45.0f, 2350.0f };
	//判定用の床の大きさ
	tnl::Vector3 wall4_size_v;
	//メッシュ
	Shared<dxe::Mesh> wall4_mesh_ = nullptr;

	//--------------------------------壁5すぐそこの横壁-----------------------------------------
	//壁の形
	const tnl::Vector3 WALL5_FORM = { 10.0f, 1.4f, 1.0f };
	//壁の座標
	const tnl::Vector3 wall5_pos_ = { 302.0f, 50.0f, 300.0f };
	//判定用の床の大きさ
	tnl::Vector3 wall5_size_v;
	//メッシュ
	Shared<dxe::Mesh> wall5_mesh_ = nullptr;

	//--------------------------------仮想壁-----------------------------------------
	//壁の形
	const tnl::Vector3 WALL_VIRTUAL_FORM = { 19.0f, 5.0f, 8.4f };
	//仮想壁の座標
	const tnl::Vector3 wall_virtual_pos_ = { 650.0f, -45.0f, 1264.0f };
	//判定用の床の大きさ
	tnl::Vector3 wall_virtual_size_v;

};
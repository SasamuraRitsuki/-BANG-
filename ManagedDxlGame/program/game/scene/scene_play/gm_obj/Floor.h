#pragma once

class Floor {
public:
	Floor(bool hard_mode);
	~Floor();

	void update(float delta_time);
	void draw(Shared<dxe::Camera> camera);

	//ゲッター
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
	//床の大きさ
	const float FLOOR_SIZE = 1000.0f;
	//動く床の大きさ
	const float MOVE_FLOOR_SIZE = 250.0f;

	//--------------------------------床1(手前)-----------------------------------------
	//床の形
	const tnl::Vector3 FLOOR1_FORM = { 1.7f, 0.25f, 1.0f };
	//床の座標
	tnl::Vector3 floor1_pos_ = { 650.0f, -155.0f, 300.0f };
	//判定用の床の大きさ
	tnl::Vector3 floor1_size_v;
	//メッシュ
	Shared<dxe::Mesh> floor1_mesh_ = nullptr;
	//--------------------------------床2(奥)-----------------------------------------
	//床の形
	const tnl::Vector3 FLOOR2_FORM = { 1.7f, 0.25f, 0.575f };
	//床の座標
	tnl::Vector3 floor2_pos_ = { 650.0f, -155.0f, 2012.4f };
	//判定用の床の大きさ
	tnl::Vector3 floor2_size_v;
	//メッシュ
	Shared<dxe::Mesh> floor2_mesh_ = nullptr;

	//--------------------------------動く床-----------------------------------------
	//床の形
	const tnl::Vector3 MOVE_FLOOR_FORM = { 1.0f, 0.25f, 1.0f };
	//床がどのくらい動くか
	const int MOVE_FLOOR_DISTANCE = 600;
	//簡単モードの時、床がどれくらいの速さで動くか
	const float MOVE_FLOOR_SPEED_EASY = 0.4f;
	//難しいモードの時、床がどれくらいの速さで動くか
	const float MOVE_FLOOR_SPEED_HARD = 0.6f;
	//判定用の床の大きさ
	tnl::Vector3 move_floor_size_v;
	//床がどれくらいの速さで動くか
	float move_floor_speed_;
	//経過時間
	float flashed_value_ = 0;
	//--------------------------------動く床1(手前)-----------------------------------------
	//床の初期座標
	tnl::Vector3 MOVE_FLOOR1_POS = { 650.0f,-61.0f,1050.0f };
	//床の座標
	tnl::Vector3 move_floor1_pos_ = MOVE_FLOOR1_POS;
	//初期位置から動いた動く床1の移動距離
	float delta_move_floor1_distance_ = 0.0f;
	//メッシュ
	Shared<dxe::Mesh> move_floor1_mesh_ = nullptr;
	//--------------------------------動く床2(奥)-----------------------------------------
	//床の初期座標
	tnl::Vector3 MOVE_FLOOR2_POS = { 650.0f,-61.0f,1400.0f };
	//床の座標
	tnl::Vector3 move_floor2_pos_ = MOVE_FLOOR2_POS;
	//初期位置から動いた動く床2の移動距離
	float delta_move_floor2_distance_ = 0.0f;
	//メッシュ
	Shared<dxe::Mesh> move_floor2_mesh_ = nullptr;



};
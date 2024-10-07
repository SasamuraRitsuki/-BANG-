#pragma once


class Bullet {
public:
	//座標、向き、向き(度数法の角度)、敵か味方か
	Bullet(const tnl::Vector3& spawn_pos, const tnl::Vector3& move_dir,const int& dir_value,const bool& enemy);
	~Bullet();

	void update(float delta_time);
	void draw(Shared<dxe::Camera> camera);

	//弾の向き
	void BulletDir(bool bullet_reflection);

	//ゲッター
	tnl::Vector3 getBulletPos() const {
		return blt_pos_;
	}
	tnl::Vector3 getBulletSize() const {
		return blt_size_v;
	}
	bool getBulletReflected() const {
		return blt_reflected_;
	}

private:

	//弾の形
	const tnl::Vector3 BULLET_FORM = { 1.0f, 1.0f, 2.0f };
	//戦車の回転の軸
	const tnl::Vector3 TANK_AXIS = { 0.0f, 1.0f, 0.0f };
	//弾の大きさ
	const float BULLET_SIZE = 15.0f;
	//弾速
	const float BULLET_SPEED = 10.0f;
	//弾の前方オフセット
	const float BULLET_FORWARD_OFFSET = 7.0f;
	//弾の向き
	const int BULLET_DIR = 1;
	//反射後の弾の向き
	const int BULLET_REFLECT_DIR = -2;
	//弾の判定用の大きさ
	tnl::Vector3 blt_size_v;

	//メッシュ
	Shared<dxe::Mesh> mesh_ = nullptr;
	//弾の座標
	tnl::Vector3 blt_pos_;
	//弾が反射されたかどうか
	bool blt_reflected_ = false;

	//弾の向き
	int blt_dir_;
	//弾の向き(Vector3)
	tnl::Vector3 move_dir_ = { 0,0,0 };

	Shared<dxe::Texture> texture = nullptr;
};
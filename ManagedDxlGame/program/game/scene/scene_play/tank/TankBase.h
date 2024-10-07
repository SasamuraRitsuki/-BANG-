#pragma once

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Wall;
class Floor;
class Spawner;
class Bullet;
class PlayerTank;

class TankBase {
protected:

	virtual void update(float delta_time) {};
	virtual void draw(Shared<dxe::Camera> camera) {};

	//壁との当たり判定
	bool TryBulletToWallCollision(std::shared_ptr<Bullet> blt, std::shared_ptr<Wall> wall);
	//共通の画像パスをロードする関数
	void LoadCommonGraph();
	//共通の音楽パスをロードする関数
	void LoadCommonSound();

	//戦車の回転の軸
	const tnl::Vector3 TANK_AXIS = { 0.0f,1.0f,0.0f };
	//戦車の進む方向(前方)
	const tnl::Vector3 TANK_DIR = { 0.0f,0.0f,1.0f };
	//重心円の半径
	const float CENTROID_RADIUS = 20.0f;
	//質量
	const float MASS = 98.0f;
	//摩擦
	const float FRICTION = 1.0f;
	//ラジアンを度に変換するための定数
	const float RAD_TO_DEG = 180.0f / static_cast<float>(M_PI);
	//死亡HP
	const int TANK_DAED_HP = 0;
	//ゲージの減少の補間回数
	const int SMOOTHING_ITERATIONS = 0;

	//------------------宣言のみ------------------
	//メッシュ
	Shared<dxe::Mesh> mesh_ = nullptr;
	//自機の共有ポインタ
	Shared<PlayerTank> player_;
	//スポナーの共有ポインタ
	Shared<Spawner> spawner_;
	//床の共有ポインタ
	Shared<Floor> floor_;
	//壁の共有ポインタ
	Shared<Wall> wall_;

	//現在のtankの位置
	tnl::Vector3 tank_pos_;
	//tankの判定用の大きさ
	tnl::Vector3 tank_size_;
	//弾の発射の音楽パス
	int shot_snd_;
	//ダメージの音楽パス(共通)
	int damage_snd_;
	//HPの画像パス(共通)
	int hp_gfx_;
	//バーの背景画像パス(共通)
	int hp_back_gfx_;
	//残りHP
	int tank_hp_;
	//死亡判定
	bool dead_ = false;
	//ダメージを受けた
	bool damaged_ = false;
	//回転中
	bool rolling_ = false;
	//ダメージを受けた瞬間
	bool damaged_moment_ = false;

	//--------摩擦もろもろ--------
	//動きの量
	tnl::Vector3 move_vel_;
	//1つ前の動きの量
	tnl::Vector3 past_move_vel_;
	//重心
	tnl::Vector3 center_of_gravity_;
	//----------------------------
public:
	//弾のリスト
	std::list<std::shared_ptr<Bullet>> bullet_;

	//セッター
	void setTankPos(tnl::Vector3 tank_pos);
	void setTankDamaged(bool damaged);
	void setTankHp(int hp);
	void setTankDamagedMoment(bool damaged_moment);
	void setPlayerPointer(std::shared_ptr<PlayerTank> player);
	void setFloorPointer(std::shared_ptr<Floor> floor);
	void setWallPointer(std::shared_ptr<Wall> wall);
	void setSpawnerPointer(std::shared_ptr<Spawner> spawner);

	//ゲッター
	tnl::Vector3 getTankPos() const;
	tnl::Vector3 getTankSize() const;
	int getTankHp() const;
	bool getTankDead() const;
	bool getTankRolling() const;
	bool getTankDamaged() const;
};
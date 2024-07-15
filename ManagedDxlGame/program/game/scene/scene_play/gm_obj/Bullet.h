#pragma once


class Bullet {
public:
	//À•WAŒü‚«AŒü‚«(“x”–@‚ÌŠp“x)A“G‚©–¡•û‚©
	Bullet(const tnl::Vector3& spawn_pos, const tnl::Vector3& move_dir,const int& dir_value,const bool& enemy);
	~Bullet();

	void update(float delta_time);
	void draw(Shared<dxe::Camera> camera);

	//’e‚ÌŒü‚«
	void BulletDir(bool bullet_reflection);

	//ƒQƒbƒ^[
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

	//’e‚ÌŒ`
	const tnl::Vector3 BULLET_FORM = { 1.0f, 1.0f, 2.0f };
	//’e‚Ì‘å‚«‚³
	const float BULLET_SIZE = 15;
	//’e‘¬
	const float BULLET_SPEED = 10;
	//’e‚ÌŒü‚«
	const int BULLET_DIR = 1;
	//”½ËŒã‚Ì’e‚ÌŒü‚«
	const int BULLET_REFLECT_DIR = -2;
	//’e‚Ì”»’è—p‚Ì‘å‚«‚³
	tnl::Vector3 blt_size_v;

	//ƒƒbƒVƒ…
	Shared<dxe::Mesh> mesh_ = nullptr;
	//’e‚ÌÀ•W
	tnl::Vector3 blt_pos_;
	//’e‚ª”½Ë‚³‚ê‚½‚©‚Ç‚¤‚©
	bool blt_reflected_ = false;

	//’e‚ÌŒü‚«
	int blt_dir_;
	//’e‚ÌŒü‚«(Vector3)
	tnl::Vector3 move_dir_ = { 0,0,0 };

	Shared<dxe::Texture> texture = nullptr;
};
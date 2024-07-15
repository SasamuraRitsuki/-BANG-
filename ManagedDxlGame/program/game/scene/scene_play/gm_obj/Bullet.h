#pragma once


class Bullet {
public:
	//���W�A�����A����(�x���@�̊p�x)�A�G��������
	Bullet(const tnl::Vector3& spawn_pos, const tnl::Vector3& move_dir,const int& dir_value,const bool& enemy);
	~Bullet();

	void update(float delta_time);
	void draw(Shared<dxe::Camera> camera);

	//�e�̌���
	void BulletDir(bool bullet_reflection);

	//�Q�b�^�[
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

	//�e�̌`
	const tnl::Vector3 BULLET_FORM = { 1.0f, 1.0f, 2.0f };
	//�e�̑傫��
	const float BULLET_SIZE = 15;
	//�e��
	const float BULLET_SPEED = 10;
	//�e�̌���
	const int BULLET_DIR = 1;
	//���ˌ�̒e�̌���
	const int BULLET_REFLECT_DIR = -2;
	//�e�̔���p�̑傫��
	tnl::Vector3 blt_size_v;

	//���b�V��
	Shared<dxe::Mesh> mesh_ = nullptr;
	//�e�̍��W
	tnl::Vector3 blt_pos_;
	//�e�����˂��ꂽ���ǂ���
	bool blt_reflected_ = false;

	//�e�̌���
	int blt_dir_;
	//�e�̌���(Vector3)
	tnl::Vector3 move_dir_ = { 0,0,0 };

	Shared<dxe::Texture> texture = nullptr;
};
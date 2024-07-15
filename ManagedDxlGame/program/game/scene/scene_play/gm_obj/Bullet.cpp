#include "../../../../dxlib_ext/dxlib_ext.h"
#include "Bullet.h"

Bullet::Bullet(const tnl::Vector3& spawn_pos, const tnl::Vector3& move_dir,const int& dir_value, const bool& enemy) {
	//�G�Ȃ�
	if (enemy) {
		texture = dxe::Texture::CreateFromFile("graphics/black1.bmp");
	}
	//�����Ȃ�
	else {
		texture = dxe::Texture::CreateFromFile("graphics/yellow1.jpg");
	}
	mesh_ = dxe::Mesh::CreateCubeMV(BULLET_SIZE);
	mesh_->setTexture(texture);
	mesh_->scl_ = BULLET_FORM;
	move_dir_ = move_dir;

	//�e���o�Ă���Ƃ���͏����O��
	blt_pos_ = spawn_pos + (move_dir * BULLET_SPEED * 7);
	//�e�̕�����������������
	mesh_->rot_ = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(static_cast<float>(dir_value)));

	mesh_->setCullingMode(DX_CULLING_RIGHT);


	blt_size_v = BULLET_FORM * BULLET_SIZE;

	//�ŏ��̒e�̌���
	BulletDir(blt_reflected_);
}

Bullet::~Bullet() {

}

void Bullet::update(float delta_time) {

	blt_pos_ += move_dir_ * BULLET_SPEED * static_cast<float>(blt_dir_);

	//���b�V���ɔ��f
	mesh_->pos_ = blt_pos_;
}

void Bullet::draw(Shared<dxe::Camera> camera) {
	mesh_->render(camera);
}

void Bullet::BulletDir(bool bullet_reflection) {

	if (!bullet_reflection) {
		blt_dir_ = BULLET_DIR;
	}
	//���˂��Ă���t������
	else{
		blt_dir_ = BULLET_REFLECT_DIR;
		blt_reflected_ = true;
	}
}